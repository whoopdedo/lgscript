/******************************************************************************
 *  LgShell.cpp
 *
 *  This file is part of LgScript
 *  Copyright (C) 2011 Tom N Harris <telliamed@whoopdedo.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *****************************************************************************/
#include <csignal>
#include <cstring>
#include <cerrno>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <limits>

#define lua_c
#define LUAX_INLINE
#include "luax.h"
#include "lua/lobject.h"
#include "lua/lmem.h"
#include "lua/lstring.h"
#include "lua/ldo.h"
#include "lua/lfunc.h"
#include "lua/lopcodes.h"
#include "lua/lundump.h"

#define LUAC_OUTPUT	"out.luac"


using namespace luax;
using namespace std;

class LgScriptArgs {
public:
	enum source { exec, require, file };
	typedef list<pair<source,string> >::const_iterator iterator;

private:
	int argc;
	const char* const* argv;
	const char* progname;

	bool version;
	bool help;
	// interactive is automatically set when there are no scripts from load or exec
	// but not if the script is read from stdin
	bool interactive;
	bool compile;
	bool optimize;
	string chunkname;

	// ordered list of scripts, require is ignored when compiling
	// when not compiling, arguments after the first file are passed to the script
	list<pair<source,string> > script;
	//list<string> script_args;
	int pre_arg, first_arg;

public:
	LgScriptArgs(int ac, const char* const* av);

	bool hasVersion(void) const { return version; };
	bool hasHelp(void) const { return help; };
	bool hasCompile(void) const { return compile; };
	bool hasOptimize(void) const { return optimize; };
	const string& getOutput(void) const { return chunkname; };
	bool hasInteractive(void) const { return interactive; };
	bool hasArgs(void) const { return first_arg < argc; };

	void showHelp(void) const;
	void showVersion(void) const;

	bool collect(void);
	size_t numScripts(void) const { return script.size(); };
	iterator begin(void) const { return script.begin(); };
	bool next(string* _s, source* _t, iterator* _i) const
	{
		if (*_i == script.end()) return false;
		*_s = (*_i)->second;
		*_t = (*_i)->first;
		++(*_i);
		return true;
	};
	int pushArgs(State& S) const;
	int argTable(State& S) const;
};

class LgScriptApp : public MainState {
private:
	LgScriptArgs args;
	int status;
	int count;

	int main(void);
	void runScripts(void);
	void runConsole(void);
	bool loadLine(void);
	State& loadFile(const char* filename);
	State& loadFile(const std::string& filename)
	{
		return loadFile(filename.c_str());
	};
	bool pushLine(bool first = false);
	const char* getPrompt(bool first);
	void traceCall(int nargs, bool clear = true);
	static int traceback(Handle L);
	Proto const* mergeFunctions(void);

public:
	LgScriptApp(int ac, const char* const* av)
		: MainState(),
		args(ac,av), status(0), count(0)
	{ };

	int getStatus(void) const { return status; };

	static int pMain(Handle _l);
};

class LgScriptWriter : public Dump {
	ofstream outfile;
public:
	virtual ~LgScriptWriter() { };
	LgScriptWriter(const string& filename);

	virtual int write(State& S, const char* p, size_t sz);
};

class LgScriptReader : public Undump {
	istream* infile;
	bool extraline;
	char buff[LUAL_BUFFERSIZE];
public:
	virtual ~LgScriptReader();
	LgScriptReader(const char* filename);

	virtual const char* read(State& S, size_t *sz);
};

LgScriptWriter::LgScriptWriter(const string& filename)
	: outfile(filename.c_str(), ios::out | ios::trunc | ios::binary)
{
	if (!outfile.is_open())
		throw FileError(strerror(errno));
}

int LgScriptWriter::write(State&, const char* p, size_t sz)
{
	if (sz != 0)
	{
		outfile.write(p, sz);
		if (outfile.bad())
			return -1;
	}
	return 0;
}

LgScriptReader::~LgScriptReader()
{
	if (infile != &cin)
		delete infile;
}

LgScriptReader::LgScriptReader(const char* filename)
{
	ifstream* finfile = NULL;
	if (filename)
	{
		infile = finfile = new ifstream(filename, ios::in);
		if (!finfile->is_open())
			throw FileError(strerror(errno));
	}
	else
		infile = &cin;

	int c = infile->get();
	if (c == '#')  /* Unix exec. file? */
	{
		extraline = true;
		infile->ignore(numeric_limits<int>::max(), '\n');
		if (!infile->eof())
			c = infile->get();
	}
	else if (c == '@')  /* DOS batch file? */
	{
		do
		{
			if (infile->eof())
				break;
			c = infile->get();
		} while (c != ' ' && c != '\n');
	}
	if (c == LUA_SIGNATURE[0] && finfile)  /* binary file? */
	{
		finfile->close();
		finfile->open(filename, ios::in | ios::binary);
		if (!finfile->is_open())
			throw FileError(strerror(errno));
		finfile->ignore(numeric_limits<int>::max(), LUA_SIGNATURE[0]);
		extraline = 0;
	}
	infile->unget();
}

const char* LgScriptReader::read(State&, size_t *sz)
{
	if (extraline)
	{
		extraline = false;
		*sz = 1;
		return "\n";
	}
	if (infile->eof())
		return NULL;
	infile->read(buff, sizeof(buff));
	*sz = infile->gcount();
	return buff;
}

LgScriptArgs::LgScriptArgs(int ac, const char* const* av)
	: argc(ac), argv(av),
	version(false), help(false), interactive(false),
	compile(false), optimize(false), pre_arg(1), first_arg(1)
{
	const char* p = strrchr(av[0], '\\');
	progname = p ? p+1 : av[0];
}

void LgScriptArgs::showVersion(void) const
{
	cerr << LUA_RELEASE << endl << LUA_COPYRIGHT << endl;
}

void LgScriptArgs::showHelp(void) const
{
	cout << "usage: " << progname << " [options] [script [args]].\n";
	cout << "Available options are:\n"
		"  -c name  compile and dump the binary chunks to " LUA_QL("name") "\n"
		"  -o name  like -c but omit redundant information so the chunk is smaller\n"
		"  -e stat  execute string " LUA_QL("stat") "\n"
		"  -l name  require library " LUA_QL("name") " (ignored when compiling)\n"
		"  -i       enter interactive mode after executing " LUA_QL("script") "\n"
		"  -h       show this help\n"
		"  -v       show version information\n"
		"  --       stop handling options\n"
		"  -        execute stdin and stop handling options\n";
}

int LgScriptArgs::argTable(State& S) const
{
	S.createTable(argc - first_arg);
	int n = 0;
	for (int i = pre_arg-1; i >= 0; i--)
	{
		S.push(argv[i]).rawSetN(n--);
	}
	n = 0;
	for (int i = first_arg; i < argc; i++)
	{
		S.push(argv[i]).rawSetN(++n);
	}
	S.setGlobal("arg");
	return n;
}

int LgScriptArgs::pushArgs(State& S) const
{
	//if (script_args.empty()) return 0;
	S.checkStack((argc - first_arg) + 3, "too many arguments to script");
	//for (list<string>::const_iterator i=script_args.begin();
	//     i != script_args.end(); ++i)
	for (int i = first_arg; i < argc; i++)
	{
		S.push(argv[i]);
	}
	return argc - first_arg;
}

// check that argument has no extra characters at the end
#define notail(x,i)	{if ((x)[i][2] != '\0') return argerror(x,i); }
static bool argerror(const char* const* arg, int i)
{
	cerr << arg[0] << ": invalid option -- " << (arg[i]+1) << endl;
	return false;
}
static bool opterror(const char* const* arg, int i)
{
	cerr << arg[0] << ": option requires an argument -- " << (arg[i]+1) << endl;
	return false;
}
bool LgScriptArgs::collect(void)
{
	bool last = false;
	int count = 0;
	script.clear();
	//script_args.clear();
	int i;
	for (i = 1; !last && argv[i] != NULL; i++) {
		pre_arg = i + 1;
		if (argv[i][0] != '-') // not an option?
		{
			count++;
			script.push_back(pair<source,string>(file,argv[i]));
			if (!compile) {
				last = true;
			}
			continue;
		}
		switch (argv[i][1]) // option
		{
		case '-':
		{
			notail(argv,i);
			pre_arg--;
			last = true;
			break;
		}
		case '\0':
		{
			count++;
			script.push_back(pair<source,string>(file,argv[i]));
			if (!compile) {
				last = true;
			}
			break;
		}
		case 'h':
		{
			help = true;
			version = true;
			break;
		}
		case 'i':
		{
			notail(argv,i);
			interactive = true;
			version = true;
			break;
		}
		case 'v':
		{
			notail(argv,i);
			version = true;
			break;
		}
		case 'o':
			optimize = true;
		case 'c':
		{
			compile = true;
			chunkname.clear();
			if (argv[i][2] == '\0') {
				i++;
				if (argv[i] != NULL &&
				    argv[i][0] == '-' && argv[i][1] != '\0')
					--i;
				else if (argv[i] != NULL)
					chunkname = argv[i];
			}
			else
				chunkname = argv[i]+2;
			if (chunkname.empty())
				chunkname = LUAC_OUTPUT;
			break;
		}
		case 'e':
		{
			notail(argv,i);
			i++;
			if (argv[i] == NULL || argv[i][0] == '-')
				return opterror(argv,i-1);
			count++;
			script.push_back(pair<source,string>(exec,argv[i]));
			break;
		}
		case 'l':
		{
			notail(argv,i);
			i++;
			if (argv[i] == NULL || argv[i][0] == '-')
				return opterror(argv,i-1);
			script.push_back(pair<source,string>(require,argv[i]));
			break;
		}
		default:
			return argerror(argv,i);
		}
	}
	if (count == 0 && !version)
	{
		interactive = true;
		version = true;
	}
	first_arg = i;
	//for (; i < argc; i++)
	//	script_args.push_back(string(argv[i]));
	return true;
}

static inline Proto* toproto(lua_State* L, int ix)
{
	return clvalue(L->top+ix)->l.p;
}

Proto const* LgScriptApp::mergeFunctions(void)
{
	if (count == 1)
		return toproto(L(), -1);

	Proto* f = luaF_newproto(L());
	setptvalue2s(L(), L()->top, f);
	incr_top(L());
	f->source = luaS_newliteral(L(), "=(lgscript)");
	f->maxstacksize = 1;
	int pc = 2 * count + 1;
	f->code = luaM_newvector(L(), pc, Instruction);
	f->sizecode = pc;
	f->p = luaM_newvector(L(), count, Proto*);
	f->sizep = count;
	pc = 0;
	int n = -count - 1;
	for (int i = 0; i < count; i++)
	{
		f->p[i] = toproto(L(), n++);
		f->code[pc++] = CREATE_ABx(OP_CLOSURE,0,i);
		f->code[pc++] = CREATE_ABC(OP_CALL,0,1,1);
	}
	f->code[pc++] = CREATE_ABC(OP_RETURN,0,1,0);
	return f;
}

static void lstop(lua_State* L, lua_Debug*)
{
	lua_sethook(L, NULL, 0, 0);
	luaL_error(L, "interrupted!");
}

static lua_State* g_L = NULL;

static void laction(int i)
{
	signal(i, SIG_DFL); // if another SIGINT happens before lstop, terminate process (default action)
	if (g_L)
		lua_sethook(g_L, lstop, LUA_MASKCALL | LUA_MASKRET | LUA_MASKCOUNT, 1);
}

int LgScriptApp::traceback(Handle L) {
	State S(L);
	if (!S.isString(1)) // 'message' not a string?
		return 1;
	if (!S.getGlobal("debug").isTable())
	{
		S.pop();
		return 1;
	}
	if (!S.getField("traceback").isFunction())
	{
		S.pop(2);
		return 1;
	}
	S.copy(1).push(2).call(2, 1);
	return 1;
}

void LgScriptApp::traceCall(int narg, bool clear)
{
	int base = getTop() - narg; // function index
	push(traceback).insert(base);
	g_L = L();
	signal(SIGINT, laction);
	try
	{
		pCall(narg, clear ? 0 : MultiRet, base);
		status = 0;
	}
	catch (Exception& err)
	{
		status = err.which();
		gcCollect();
		signal(SIGINT, SIG_DFL);
		g_L = NULL;
		remove(base);
		throw;
	}
	signal(SIGINT, SIG_DFL);
	g_L = NULL;
	remove(base);
}

const char* LgScriptApp::getPrompt(bool first)
{
	getGlobal(first ? "_PROMPT" : "_PROMPT2");
	const char* p = asString();
	if (p == NULL)
		p = first? LUA_PROMPT : LUA_PROMPT2;
	pop();
	return p;
}

bool LgScriptApp::pushLine(bool first)
{
	char buf[LUA_MAXINPUT];
	string line;
	cout << getPrompt(first);
	do
	{
		cin.clear();
		cin.getline(buf, LUA_MAXINPUT);
		line += buf;
	} while (cin.fail() && !cin.eof());
	if (first && line[0] == '=')
		line.replace(0, 1, "return ", 7);
	push(line);
	return !cin.eof();
}

bool LgScriptApp::loadLine(void) {
	setTop(0);
	if (!pushLine(true)) // no input
		return true;
	for (;;) // repeat until gets a complete line
	{
		try
		{
		loadString(asString(1), objLen(1), "=stdin");
		break;
		}
		catch (SyntaxError& err)
		{
			size_t pos = err.msg().rfind(LUA_QL("<eof>"));
			if (pos != err.msg().size() - (sizeof(LUA_QL("<eof>")) - 1))
				throw;
			pop();
		}
		if (!pushLine()) // more input?
			return true;
		push("\n", 1); // add a new line...
		insert(-2); // ...between the two lines
		concat(3); // join them
	}
	remove(1);
	return false;
}

State& LgScriptApp::loadFile(const char* filename)
{
	int fnameindex = getTop() + 1;  /* index of filename on the stack */
	if (filename == NULL)
		push("=stdin");
	else
		pushFormat("@%s", filename);
	LgScriptReader reader(filename);
	load(&reader, asString(fnameindex));
	remove(fnameindex);
	return *this;
}

void LgScriptApp::runConsole(void)
{
	cin.tie(&cout);
	bool last = false;
	while (!last)
	{
		try
		{
		status = 0;
		last = loadLine();
		if (!isFunction()) continue;
		count++;
		if (!args.hasCompile())
		{
			traceCall(0, 0);
			if (status == 0 && getTop() > 0)
			{
				getGlobal("print");
				insert(1);
				pCall(getTop()-1, 0);
			}
		}
		}
		catch (Exception& err)
		{
			cerr << err.what() << endl;
			status = err.which();
			pop();
			gcCollect();
		}
	}
}

void LgScriptApp::runScripts(void)
{
	try
	{
	if (!args.hasCompile())
		args.argTable(*this);
	LgScriptArgs::source source;
	string script;
	LgScriptArgs::iterator i = args.begin();
	while (args.next(&script, &source, &i))
	{
		switch (source)
		{
		case LgScriptArgs::exec:
			loadString(script, "=(command line)");
			if (isFunction()) count++;
			if (!args.hasCompile())
				traceCall(0);
			break;
		case LgScriptArgs::require:
			if (!args.hasCompile())
			{
				getGlobal("require");
				push(script);
				traceCall(1);
			}
			break;
		case LgScriptArgs::file:
			if (script == "-")
				loadFile(NULL);
			else
				loadFile(script);
			if (isFunction()) count++;
			if (!args.hasCompile())
				traceCall(args.pushArgs(*this));
			break;
		}
	}
	}
	catch (Exception& err)
	{
		cerr << err.what() << endl;
		status = err.which();
		pop();
		gcCollect();
	}
}

int LgScriptApp::main(void)
{
	if (!args.collect())
	{
		args.showHelp();
		status = 1;
		return 0;
	}
	if (args.hasVersion())
	{
		args.showVersion();
		//return 0;
	}
	if (args.hasHelp())
	{
		args.showHelp();
		status = 0;
		return 0;
	}
	if (!args.hasCompile())
	{
		gcStop();
		openLibs();
		// load lgscript module
		gcRestart();
	}
	else
	{
		checkStack(args.numScripts(), "too many input files");
	}
	runScripts();
	if (status != 0) return 0;
	if (args.hasInteractive())
		runConsole();
	if (args.hasCompile())
	{
		Proto const* chunk = mergeFunctions();
		LgScriptWriter writer(args.getOutput());
		lua_lock(L());
		luaU_dump(L(), chunk, reinterpret_cast<Writer>(LgScriptWriter::writer), &writer, args.hasOptimize());
		lua_unlock(L());
	}
	return 0;
}

int LgScriptApp::pMain(Handle _l)
{
	State S(_l);
	LgScriptApp* app = reinterpret_cast<LgScriptApp*>(S.toUserdata(1));
	return app->main();
}

int main(int argc, char* argv[])
{
	try
	{
		LgScriptApp app(argc, argv);
		app.pCall(LgScriptApp::pMain, &app);
		return (app.getStatus() != 0);
	}
	catch (Exception& e)
	{
		cerr << "Unhandled error in script: " << e.what() << endl;
		return 125;
	}
	catch (exception& e)
	{
		cerr << "Unhandled error in application: " << e.what() << endl;
		return 126;
	}
	catch (...)
	{
		cerr << "Unexpected error!" << endl;
		return 127;
	}
	return 0;
}
