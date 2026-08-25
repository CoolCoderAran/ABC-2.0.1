#ifndef ABC_BHDRS_BVIS_HEADER
#define ABC_BHDRS_BVIS_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/* bvis.h: Public declarations for Visible Procedures */

#include <stdbool.h>

#include "b.h"
#include "bint.h"
#include "bmem.h"
#include "btr/i1btr.h"
#include "ehdrs/cell.h"
#include "ehdrs/queu.h"
#include "ehdrs/supr.h"
#include "ehdrs/tabl.h"
#include "ihdrs/i1num.h"
#include "ihdrs/i3cen.h"
#include "ihdrs/i3env.h"
#include "ihdrs/i3typ.h"
#include "ihdrs/i2gen.h"
#include "stc/i2stc.h"
#include "ihdrs/i2par.h"
#include "ihdrs/i2exp.h"

/* b1grab.c */
Visible Procedure release(value v);
Visible Procedure uniql(value *ll);
Visible Procedure regrab(value *v, short len);
Visible Procedure rrelease(value v);

/* b1file.c */
Visible Procedure free_path(char *str);
Visible Procedure endfile(void);
Visible bool readfile(environment *ep, string filename, int line, bool creating);

/* b1memo.c */
Visible Procedure regetmem(char **v, size_t syze);
Visible Procedure freemem(ptr p);
Visible Procedure bufgrow(bufadm *bp);
Visible Procedure bufpush(bufadm *bp, char c);
Visible Procedure bufcpy(bufadm *bf, char *s);
Visible Procedure bufreinit(bufadm *bp);
Visible Procedure bufinit(bufadm *bp);
Visible Procedure buffree(bufadm *bp);
Visible Procedure bufncpy(bufadm *bp, char *s, size_t len);

/* b1mess.c */
Visible Procedure putmess(int m);
Visible Procedure putSmess(int m, string s);
Visible Procedure putDSmess(int m, int d, string s);
Visible Procedure initmess(void);
Visible Procedure endmess(void);
Visible Procedure put2Cmess(int m, char c1, char c2);

/* b1outp.c */
Visible Procedure c_putstr(string s);
Visible Procedure c_putdata(string data);
Visible Procedure c_putnewline(void);
Visible Procedure c_flush(void);
Visible Procedure init_interpreter_output(int height, int width);
Visible Procedure re_interpreter_output(void);
Visible Procedure putnewline(FILE *file);
Visible Procedure putstr(FILE *file, string s);
Visible Procedure doflush(FILE *file);
Visible Procedure putchr(FILE *file, char ch);
Visible int getwinwidth(void);
Visible int getwincol(void);

/* e1cell.c */
Visible Procedure discard(register cell *p);
Visible cell *replist(cell *tops, cell *rep, int oldlno, register int oldlcnt);
Visible cell *build(path p, register int lcnt);
Visible cell *gettop(cell *tops);

/* e1comm.c */
Visible Procedure abced_file(
	string filename,
	intlet errline,
	literal kind,
	bool creating,
	bool *changed);
Visible Procedure initbed(void);
Visible Procedure endbed(void);
Visible char *ed_line(literal kind, int indent);

/* e1deco.c*/
Visible Procedure delfocus(register path *pp);
Visible bool deltext(register environment *ep);
Visible bool delbody(register environment *ep);
Visible bool copyinout(register environment *ep);
Visible value copyout(register environment *ep);
Visible bool ishole(register environment *ep);

/* e1edoc.c */
Visible Procedure dumpev(register environment *ep, string m);
Visible Procedure clrenv(environment *ep);
Visible Procedure higher(register environment *ep);
Visible Procedure dbmess(register environment *ep);
Visible bool dofile(environment *ep, string filename, int linenumber, literal kind, bool creating, bool *changed);
Visible bool suspendabc(void);
Visible bool editdocument(environment *ep, bool bad_file);
Visible bool savequeue(value v, string filename);

/* e1erro.c*/
Visible Procedure ederr(int m);
Visible Procedure ederrS(int m, string s);
Visible Procedure ederrC(int m, char c);
Visible Procedure edmessage(string s);
[[noreturn]]
Visible Procedure asserr(char *file, int line);
Visible Procedure debug(string msg);
Visible Procedure debug1(string fmt, char c);
Visible Procedure debug1i(string fmt, int i);
Visible Procedure debug2(string ftm, int i, int j);
Visible Procedure debug5(string fmt, int i, int j, int k, int l, int m);
Visible Procedure stsline(
	int totlines,
	int topline,
	int scrlines,
	value copybuffer,
	bool recording);
Visible Procedure enderro(void);
Visible Procedure init_erro(void);
Visible Procedure end_erro(void);

/* e1eval.c*/
Visible Procedure evalcoord(
	register node n,
	register int jch,
	int *py,
	int *px,
	int *plevel);
Visible int fwidth(register string str);
Visible int evalwidth(register node n);

/* e1getc.c*/
Visible Procedure initgetc(void);
Visible Procedure endgetc(void);
Visible Procedure pollinterrupt(void);
Visible Procedure initkeys(void);
Visible Procedure addkeydef(
	int code,
	int deflen,
	string def,
	string rep,
	string name);
Visible Procedure dumpkeys(string where);
Visible Procedure initoperations(void);
Visible Procedure undefine(int code, int deflen, string def);
Visible int getoperation(void);
Visible Procedure pollinterrupt(void);

/* e1goto.c*/
Visible Procedure gotofix(environment *ep, int y, int x);
Visible bool gotocursor(environment *ep);
Visible bool gotomouse(environment *ep);
Visible bool gotoyx(register environment *ep, register int y, register int x);

/* e1gram.c*/
Visible Procedure setroot(int isym);
Visible Procedure initgram();
Visible Procedure initclasses(void);
Visible Procedure endnoderepr(void);
Visible bool isinclass(register int sym, struct classinfo *ci);
Visible string *noderepr(register node n);
Visible string symname(int sym);
Visible int nametosym(register string str);
Visible bool allowed(register path p, register int sym);
Visible bool samelevel(register int sym, register int sym1);
Visible bool issublist(register int sym);
Visible bool allows_colon(int sym);
Visible node gram(register int sym);

/* e1help.c */
Visible bool help(void);
Visible bool ask_for(int nr);

/* e1lexi.c*/
Visible bool maystart(char c, int ilex);
Visible bool maycontinue(char c, int ilex);

/* e1inse.c*/
Visible bool insguess(path *pp, char c, environment *ep);
Visible bool mayinsert(node n, int ich, int s2, register char c);
Visible bool soften(environment *ep, string *pstr, int alt_c);
Visible bool resuggest(environment *ep, string *pstr, int alt_c);

/* e1ins2.c*/
Visible bool ins_char(register environment *ep, int c, int alt_c);
Visible bool ins_newline(register environment *ep, bool reading_file);

/* e1line.c*/
Visible bool eqlines(node n1, node n2);
Visible int linelen(node n);
Visible bool nextline(register path *pp);
Visible int lineno(register environment *ep);
Visible int colno(environment *ep);
Visible Procedure oneline(register environment *ep);

/* e1move.c*/
Visible bool previous(environment *ep);
Visible bool nextarrow(environment *ep);
Visible bool leftarrow(environment *ep);
Visible bool ritearrow(environment *ep);
Visible bool uparrow(environment *ep);
Visible bool downarrow(environment *ep);
Visible bool upline(register environment *ep);
Visible bool downline(register environment *ep);
Visible bool acceptcommand(environment *ep);

/* e1node.c*/
Visible node nodecopy(node n);
Visible Procedure noderelease(node n);
Visible Procedure nodeuniql(node *pn);
Visible node newnode(
	register int nch,
	register int sym,
	register node children[]);
Visible int nodewidth(node n);
Visible Procedure setchild(register node *pn, register int i, register node n);
Visible path pathcopy(path p);
Visible Procedure pathrelease(path p);
Visible Procedure pathuniql(path *pp);
Visible path newpath(register path pa, register node n, register int i);
Visible Procedure markpath(register path *pp, register markbits new);
Visible Procedure unmkpath(register path *pp, register int del);
Visible Procedure treereplace(register path *pp, register node n);
Visible bool up(register path *pp);
Visible bool downi(register path *pp, register int i);
Visible bool downrite(register path *pp);
Visible bool left(register path *pp);
Visible bool rite(register path *pp);
Visible Procedure top(register path *pp);
Visible bool nextnode(register path *pp);
Visible Procedure firstleaf(register path *pp);
Visible bool nextleaf(register path *pp);
Visible bool prevnode(register path *pp);
Visible Procedure lastleaf(register path *pp);
Visible bool prevleaf(register path *pp);
Visible bool nextmarked(register path *pp, register markbits x);
Visible bool firstmarked(register path *pp, register markbits x);
Visible bool prevmarked(register path *pp, register markbits x);
Visible int pathlength(register path p);
Visible Procedure putintrim(
	register value *pn,
	register int head,
	register int tail,
	register string str);
Visible Procedure touchpath(register path *pp);

/* e1outp.c*/
Visible Procedure savefocus(register environment *ep);
Visible Procedure setfocus(register cell *tops);
Visible Procedure startactupdate(bool nofocus);
Visible Procedure endactupdate(void);
Visible Procedure outline(register cell *p, register int lineno);

/* e1que1.c*/
Visible queue qcopy(queue q);
Visible Procedure qrelease(queue q);
Visible Procedure joinqueues(register queue *pq, register queue q);
Visible Procedure preptoqueue(node n, register queue *pq);
Visible Procedure addtoqueue(register queue *pq, register node n);
Visible Procedure stringtoqueue(register string str, register queue *pq);
Visible Procedure addstringtoqueue(register queue *pq, register string str);
Visible node queuebehead(register queue *pq);
Visible Procedure splitnode(register node n, register queue *pq);
Visible bool resttoqueue(register path *pp, register queue *pq);
Visible Procedure nosuggtoqueue(register environment *ep, queue *pq);
Visible bool issuggestion(register environment *ep);
Visible bool fitnode(register path *pp, register node n);
Visible int fitstring(register path *pp, register string str, int alt_c);
Visible Procedure fixfocus(register environment *ep, register int len);
Visible bool spacefix(environment *ep);
Visible Procedure subsettoqueue(
	register node n,
	register int s1,
	register int s2,
	register queue *pq);
Visible string querepr(value qv);
Visible Procedure dumpqueue(queue *pq, string m);

/* e1que2.c*/
Visible bool move_on(register environment *ep);
Visible bool ins_queue(register environment *ep, register queue *pq, register queue *pq2);
Visible bool app_queue(environment *ep, queue *pq);
Visible bool fix_move(register environment *ep);
Visible bool ins_string(
	register environment *ep,
	/*auto*/ string str,
	register queue *pq,
	int alt_c);

Visible int joinstring(
	path *pp,
	register string str,
	register bool spflag,
	int alt_c,
	bool mayindent);
Visible Procedure qshow(queue q, string where);

/* e1save.c*/
Visible bool save(path p, string filemae);
Visible char *senddoc(path p);

/* e1scrn.c*/

Visible Procedure actupdate(value copybuffer, bool recording, bool lasttime /* Yes if called from final screen update */);
Visible Procedure virtupdate(environment *oldep, environment *newep, int highest);
Visible Procedure initterm(void);
Visible Procedure initvterm(void);
Visible Procedure endterm(void);
Visible Procedure endshow(void);
Visible bool backtranslate(int *py, int *px);
Visible Procedure setindent(int x);
Visible Procedure cmdprompt(string prompt);

/* e1spos.c */
Visible Procedure initpos(void);
Visible Procedure endpos(void);
Visible int getpos(char *fname);
Visible Procedure savpos(char *fname, environment *ep);
Visible Procedure delpos(char *fname);
Visible Procedure movpos(char *ofname, char *nfname);

/* e1sugg.c */
Visible Procedure initcensugg(void);
Visible Procedure initsugg(void);
Visible Procedure endsugg(void);
Visible Procedure addsugg(string str, int builtin);
Visible bool setsugg(
	path *pp,
	char c,
	environment *ep,
	bool colon_allowed);
Visible bool newsugg(environment *ep, string *pstr, int alt_c);
Visible Procedure killsugg(environment *ep, string *pstr);
Visible Procedure acksugg(environment *ep);
Visible bool newhowsugg(environment *ep, string *pstr, int alt_c);
Visible Procedure ackhowsugg(environment *ep);
Visible Procedure killhowsugg(environment *ep);
Visible Procedure check_last_unit(environment *ep, int curr);
Visible Procedure readsugg(path p);
Visible Procedure writesugg(path p);
Visible Procedure endclasses(void);

/* e1supr.c */
Visible int lenitem(register environment *ep);
Visible Procedure grow(environment *ep, bool deleting);
Visible Procedure subgrow(register environment *ep, bool ignorespaces, bool deleting);
Visible Procedure shrink(register environment *ep);
Visible Procedure growsubset(environment *ep);
Visible Procedure subgrsubset(register environment *ep, bool ignorespaces);
Visible Procedure /* Ought to be Hidden */ shrsubset(register environment *ep);
Visible Procedure ritevhole(register environment *ep);
Visible Procedure leftvhole(register environment *ep);
Visible Procedure s_up(register environment *ep);
Visible Procedure s_downi(register environment *ep, register int i);
Visible Procedure s_down(register environment *ep);
Visible Procedure s_downrite(register environment *ep);
Visible Procedure s_left(register environment *ep);
Visible Procedure s_rite(register environment *ep);
Visible bool checkep(register environment *ep);
Visible bool nextnnitem(register environment *ep);
Visible bool prevnnitem(register environment *ep);
Visible Procedure firstnnitem(register environment *ep);
Visible Procedure fixit(register environment *ep);
Visible bool allspaces(register string str);
Visible int focwidth(register environment *ep);
Visible int focoffset(register environment *ep);
Visible int focchar(environment *ep);
Visible int nodechar(node n);
Visible int focindent(environment *ep);
Visible Procedure emove(environment *s, environment *d);
Visible Procedure ecopy(environment *s, environment *d);
Visible Procedure erelease(environment *ep);
Visible bool ev_eq(environment *l, environment *r);

/* e1wide.c */
Visible bool widen(register environment *ep, bool deleting);
Visible bool narrow(register environment *ep);
Visible bool extend(register environment *ep);
Visible bool rnarrow(register environment *ep);

/* i1nua.c */
Visible Procedure app_print(FILE *fp, real v);
Visible bool about2_to_integral(real ru, value v, real *rv);

/* i1nuc.c */
Visible Procedure printnum(FILE *fp, value v);

/* i1num.c */
Visible bool large(value v);
Visible int intval(value v);
Visible int propintlet(int i);
Visible bool integral(value v);
Visible value mk_exact(integer p, integer q, int len);
Visible relation numcomp(value u, value v);
Visible bool is_increment(value a, value b);
Visible value tento(int n);
Visible double numval(value u);
Visible double numhash(value v);
Visible Procedure set_random(value v);
Visible value rangom(void);
Visible Procedure initnum(void);
Visible Procedure endnum(void);
Visible value grab_num(register int len);
Visible value grab_rat(register int len);
Visible value regrab_num(value v, register int len);
Visible unsigned numsyze(short len, int *nptrs);
Visible Procedure set_random(value v);
Visible Procedure initapp();
Visible Procedure endapp();

/* i1nur.c */
Visible rational mk_rat(integer x, integer y, int len, bool simplify);
Visible rational rat_sum(rational u, rational v);
Visible rational rat_diff(register rational u, register rational v);
Visible rational rat_neg(register rational u);
Visible rational rat_prod(register rational u, register rational v);
Visible rational rat_quot(register rational u, register rational v);
Visible rational rat_power(rational a, integer n);
Visible rational rat_zero(void);
Visible relation rat_comp(rational a, rational b);
Visible Procedure rat_init(void);
Visible Procedure endrat(void);

/* i1tra.c */
Visible value mkchar(char c);
Visible value stripped(value t);
Visible value upper(value t);
Visible value lower(value t);
Visible value choice(value train);
Visible value split(value t);
Visible Procedure ins_range(value lwb, value upb, value *pl);

/* i2ana.c */
Visible Procedure analyze(parsetree t, value *targs);
Visible Procedure cleanup(void);

/* i2cmd.c */
Visible bool is_comment(value *v);
Visible bool control_command(char *kw, parsetree *v);
Visible bool control_command(char *kw, parsetree *v);
Visible bool term_com(char *kw, parsetree *v);
Visible bool simple_command(char *kw, parsetree *v, value *c);

/* i2dis.c */
Visible Procedure display(FILE *file, parsetree v, bool one_line);

/* i2exp.c */
Visible Procedure do_dya(expadm *adm, value v);
Visible Procedure reduce(expadm *adm);
Visible Procedure selection(txptr q, parsetree *v);
Visible Procedure initexp(expadm *adm, int n, char level);
Visible Procedure endstack(expadm *adm);
Visible Procedure push_item(expadm *adm, parsetree v);
Visible Procedure trim_target(txptr q, parsetree *v);
Visible bool tag_operator(txptr q, value *v);

/* i2fix.c */
Visible Procedure f_eunparsed(parsetree *pt);
Visible Procedure f_cunparsed(parsetree *pt);
Visible Procedure f_trim_target(parsetree v, char trim);

/* i2gen.c */
Visible Procedure jumpto(parsetree t);
Visible Procedure fix(parsetree *pt, char flag);
Visible Procedure hold(struct state *pl);
Visible Procedure let_go(struct state *pl);
Visible Procedure fix_nodes(parsetree *pt, parsetree *code);

/* i2syn.c */
Visible Procedure skipsp(txptr *tx0);
Visible Procedure findceol(void);
Visible Procedure req(string s, txptr q, txptr *ftx, txptr *ttx);
Visible Procedure upto(txptr q, string s);
Visible Procedure need(string s);
Visible Procedure veli(void);
Visible Procedure upto1(txptr q, int m);
Visible Procedure initsyn(void);
Visible Procedure endsyn(void);
Visible Procedure first_ilev(void);
Visible bool ateol(void);
Visible bool is_cmdname(txptr q, char **name);
Visible bool find(string s, txptr q, txptr *ftx, txptr *ttx);

Visible value cr_text(txptr p, txptr q);
Visible bool findrel(txptr q, txptr *ftx);
Visible bool findtrim(txptr q, txptr *first);
Visible bool is_tag(value *v);
Visible bool is_abcname(value name);
Visible char *keyword(void);
Visible bool is_keyword(char **kw);
Visible bool findkw(txptr q, txptr *ftx);
Visible bool nothing(txptr q, int m);
Visible bool _nwl_sign(void);
Visible bool _times_sign(void);
Visible bool _over_sign(void);
Visible bool _power_sign(void);
Visible bool _numtor_sign(void);
Visible bool _denomtor_sign(void);
Visible bool _join_sign(void);
Visible bool _less_than_sign(void);
Visible bool _greater_than_sign(void);
Visible bool dyamon_sign(value *v);
Visible bool dya_sign(value *v);
Visible bool mon_sign(value *v);
Visible bool texdis_sign(value *v);

/* i3bws.c */
Visible Procedure lst_wss(void);
Visible Procedure goto_ws(void);
Visible Procedure initbws(void);
Visible Procedure endbws(void);

/* i3com.c */
Visible Procedure idelpos(value fname);
Visible Procedure imovpos(value ofname, value nfname);
Visible bool f_edit(value fname, intlet errline, literal kind, bool creating);
Visible bool cmdline(literal kind, bufadm *bp, int indent);

/* i3env.c */
Visible Procedure setprmnv(void);
Visible Procedure sv_context(context *sc);
Visible Procedure set_context(context *sc);
Visible Procedure initenv(void);
Visible Procedure endenv(void);
Visible Procedure e_replace(value v, value *t, value k);
Visible Procedure e_delete(value *t, value k);
Visible Procedure re_env(void);
Visible Procedure lst_ttgs(void);
Visible Procedure initprmnv(void);
Visible Procedure extbnd_tags(value btl, envtab et);
Visible Procedure sethowtoname(value v);
Visible bool in_env(value tab, value ke, value **aa);

/* i3err.c */
[[noreturn]]
Visible Procedure bye(int ex);
[[noreturn]]
Visible Procedure immexit(int status);
Visible Procedure putserr(string s);
Visible Procedure flusherr(void);
[[noreturn]]
Visible Procedure syserr(int m);
Visible Procedure memexh(void);
Visible Procedure pprerrV(int m, value v);
Visible Procedure pprerr(int m);
Visible Procedure parerrV(int m, value v);
Visible Procedure parerr(int m);
Visible Procedure fixerrV(int m, value v);
Visible Procedure fixerr(int m);
Visible Procedure interr(int m);
Visible Procedure int_signal(void);
Visible Procedure fpe_signal(void);
Visible Procedure putsSerr(string fmt, string s);
Visible Procedure putsDSerr(string fmt, int d, string s);
Visible Procedure puts2Cerr(string fmt, char c1, char c2);
Visible Procedure putsCerr(string fmt, char c);
Visible Procedure interrV(int m, value v);
Visible Procedure initfmt(void);
Visible Procedure initerr(void);
Visible Procedure re_errfile(void);
Visible Procedure checkerr(void);
Visible Procedure typerrV(int m, value v);
Visible char *getfmtbuf(string fmt, int n);

/* i3fil.c */
Visible Procedure f_delete(char *file);
Visible Procedure f_close(FILE *usrfile);
Visible Procedure f_rename(value fname, value nfname);
Visible unsigned f_size(FILE *file);
Visible bool f_interactive(FILE *file);
Visible char *f_getline(FILE *file);
Visible value new_fname(value name, literal type);
Visible value mkabcname(char *name);

/* i3fpr.c */
Visible Procedure initfpr(void);
Visible Procedure endfpr(void);
Visible bool is_zerfun(value t, value *f);
Visible bool is_monfun(value t, value *f);
Visible bool is_dyafun(value t, value *f);
Visible bool is_zerprd(value t, value *p);
Visible bool is_monprd(value t, value *p);
Visible bool is_dyaprd(value t, value *p);
Visible value pre_fun(value nd1, intlet pre, value nd2);
Visible bool pre_prop(value nd1, intlet pre, value nd2);
Visible value nowisthetime(void);

/* i3imm.c */
Visible Procedure process(void);

/* i3ini.c */
Visible Procedure endall(void);
Visible Procedure pre_init(void);
Visible Procedure checkfileargs(int argc, char **argv);
Visible Procedure init(void);
Visible Procedure abc(int argc, char **argv);
Visible Procedure crashend(void);

/* i3int.c */
Visible Procedure execthread(parsetree start);
Visible value evalthread(parsetree start);

/* i3in2.c */
Visible Procedure load_global(value v, value name, bool err);
Visible Procedure nl(value n);

/* i3loc.c */
Visible Procedure check_location(loc l);
Visible Procedure put(value v, loc l);
Visible Procedure put_with_check(value v, loc l);
Visible Procedure l_delete(loc l);
Visible Procedure l_insert(value v, loc l);
Visible Procedure l_remove(value v, loc l);
Visible Procedure bindlocation(loc l);
Visible Procedure unbind(loc l);
Visible Procedure l_del(loc l);

/* i3scr.c */
Visible Procedure flushout(void);
Visible Procedure wri(FILE *fp, value v, bool coll, bool outer, bool perm);
Visible Procedure oline(void);
Visible Procedure writ(value v);
Visible Procedure writnewline(void);
Visible Procedure read_eg(loc l, btype t);
Visible Procedure read_raw(loc l);
Visible bool read_ioraw(value *v);
Visible Procedure re_screen(void);
Visible Procedure init_scr(void);
Visible Procedure end_scr(void);
Visible Procedure re_outfile(void);
Visible Procedure vs_ifile(void);
Visible bool is_intended(int m);

/* i3sou.c */
Visible Procedure del_target(value name);
Visible Procedure putval(value v, char *dir, char *name, literal ct, bool silently);
Visible Procedure puttarval(value v, char *dir, value fname, value tname, bool silently);
Visible Procedure put_perm(void);
Visible Procedure create_unit(void);
Visible Procedure edit_unit(void);
Visible Procedure edit_target(void);
Visible Procedure initperm(void);
Visible Procedure endperm(void);
Visible Procedure initsou(void);
Visible Procedure endsou(void);
Visible Procedure lst_uhds(void);
Visible Procedure rem_unit(parsetree u, wsenvptr wse);
Visible Procedure def_target(value name, value t);
Visible Procedure clear_perm(void);

/* i3sta.c */
Visible Procedure push(value v);
Visible Procedure ret(void);
Visible Procedure call_refinement(value name, parsetree def, bool test);
Visible Procedure x_user_command(value name, parsetree actuals, value def);
Visible Procedure endsta(void);

/* i3typ.c */
Visible Procedure must_agree(btype t, btype u, int m);

/* e1etex.c */
Visible Procedure e_fstrval(string s);
Visible Procedure e_concto(value *s, value t);

/* i1btr.c */
Visible Procedure relbtree(btreeptr pnode, literal it);
Visible Procedure uniqlbtreenode(btreeptr *pptr, literal it);

/* i1obj.c */
Visible Procedure rel_subvalues(value v);

/* i1tex.c */
Visible Procedure endstrval(void);
Visible Procedure fstrval(string s);
Visible Procedure concato(value *v, value t);
Visible Procedure convtext(Procedure (*outproc)(char c), value v, char quote);

/* i1lta.c */
Visible Procedure cpynptrs(btreeptr *to, btreeptr *from, int n);
Visible Procedure movnitms(itemptr to, itemptr from, intlet n, intlet iw);
Visible Procedure cpynitms(itemptr to, itemptr from, intlet n, intlet it);
Visible relation comp_tlt(value u, value v);
Visible relation comp_text(value u, value v) ;

/* i2tca.c */
Visible Procedure type_check(parsetree v);
Visible Procedure initstc(void);
Visible Procedure endstc(void);
Visible Procedure del_types(void);
Visible Procedure adjust_types(bool no_change);
Visible Procedure rectypes(void);
Visible Procedure put_types(void);

/* i2tce.c */
Visible Procedure start_vars(void);
Visible Procedure add_var(polytype tvar);
Visible Procedure end_vars(void);
Visible Procedure setreprtable(void);
Visible Procedure starterrvars(void);
Visible Procedure enderrvars(void);
Visible Procedure badtyperr(polytype a, polytype b);
Visible Procedure delreprtable(void);
Visible Procedure adderrvar(polytype t);

/* i2tcp.c */
Visible Procedure putsubtype(polytype sub, polytype u, intlet isub);
Visible Procedure new_externals(void);
Visible Procedure p_release(polytype u);
Visible Procedure repl_type_of(polytype u, polytype p);
Visible Procedure usetypetable(value t);
Visible Procedure deltypetable(void);
Visible Procedure initpol(void);
Visible Procedure endpol(void);

/* i2tcu.c */
Visible Procedure unify(polytype a, polytype b, polytype *pu);

/* i4bio.c */
Visible Procedure abcio(int type, char *tabname);
Visible Procedure bioerr(int m);
Visible Procedure bioerrV(int m, value v);

/* i4grp.c */
Visible Procedure rec_wsgroup(void);

/* i4inp.c */
Visible Procedure abcinput(char *name_arg);

/* i4lis.c */
Visible Procedure abclist(FILE *ofp);

/* i4out.c */
Visible Procedure abcoutput(char *name_arg);

/* i4pack.c */
Visible Procedure abcpack(FILE *ofp);

/* i4rec.c */
Visible Procedure rec_workspace(void);
Visible Procedure rec_suggestions(void);

/* trm.c */
Visible int trmstart(int *plines, int *pcols, int *pflags);
Visible Procedure trmend(void);
Visible Procedure trmundefined(void);
Visible Procedure trmsense(string sense, string format, int *py, int *px);
Visible Procedure trmputdata(int yfirst, int ylast, register int indent, register string data, register string mode);
Visible Procedure trmscrollup(register int yfirst, register int ylast, register int by);
Visible Procedure trmsync(int y, int x);
Visible Procedure trmbell(void);
Visible Procedure trmshow(char *s);
Visible int trminput(void);
Visible int trmavail(void);
Visible int trmsuspend(void);

#endif /* HEADER GUARD */
