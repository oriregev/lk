#ifndef __lk_absyn_h
#define __lk_absyn_h

#include <vector>

#if defined(WIN32)&&defined(__DLL__)
#define LKEXPORT __declspec(dllexport)
#define LKTEMPLATE
#else
#define LKEXPORT
#define LKTEMPLATE extern
#endif

#include "lk_str.h"

namespace lk {

	extern int _node_alloc;
	
	class LKEXPORT attr_t
	{
	public:
		virtual ~attr_t() {  };
	};

	class LKEXPORT node_t
	{
	private:
		int m_line;
	public:
		attr_t *attr;
		node_t(int ln) : m_line(ln), attr(0) { _node_alloc++; }
		virtual ~node_t() { _node_alloc--; if (attr) delete attr; }
		inline int line() { return m_line; }
	};
	
	class LKEXPORT list_t : public node_t
	{
	public:
		node_t *item;
		list_t *next;
		list_t( int line, node_t *i, list_t *n ) : node_t(line), item(i), next(n) {  };
		virtual ~list_t() { if (item) delete item; if (next) delete next; }
	};
					
	class LKEXPORT iter_t : public node_t
	{
	public:
		node_t *init, *test, *adv, *block;
		iter_t( int line, node_t *i, node_t *t, node_t *a, node_t *b ) : node_t(line), init(i), test(t), adv(a), block(b) { }
		virtual ~iter_t() { if (init) delete init; if (test) delete test; if (adv) delete adv; if (block) delete block; }
	};
	
	class LKEXPORT cond_t : public node_t
	{
	public:
		node_t *test, *on_true, *on_false;
		cond_t(int line, node_t *t, node_t *ot, node_t *of) : node_t(line), test(t), on_true(ot), on_false(of) { }
		virtual ~cond_t() { if (test) delete test; if (on_true) delete on_true; if (on_false) delete on_false; }
	};
	
	class LKEXPORT expr_t : public node_t
	{
	public:
		enum {
			INVALID,
			
			PLUS,
			MINUS,
			MULT,
			DIV,
			INCR,
			DECR,
			DEFINE,
			ASSIGN,
			LOGIOR,
			LOGIAND,
			NOT,
			EQ,
			NE,
			LT,
			LE,
			GT,
			GE,
			EXP,
			NEG,
			INDEX,
			HASH,
			CALL,
			THISCALL,
			RETURN,
			EXIT,
			BREAK,
			CONTINUE,
			SIZEOF,
			KEYSOF,
			TYPEOF,
			INITVEC,
			INITHASH
		};
		int oper;
		node_t *left, *right;
		const char *operstr();
		expr_t(int line, int op, node_t *l, node_t *r) : node_t(line), oper(op), left(l), right(r) {  }
		virtual ~expr_t() { if (left) delete left; if (right) delete right; }
	};
				
	class LKEXPORT iden_t : public node_t
	{
	public:
		lk_string name;
		bool local;
		iden_t(int line, const lk_string &n, bool loc) : node_t(line), name(n), local(loc) {  }
		virtual ~iden_t() { }
	};
				
	class LKEXPORT constant_t : public node_t
	{
	public:
		double value;
		constant_t(int line, double v) : node_t(line), value(v) {  }
		virtual ~constant_t() {  }
	};
	
	class LKEXPORT literal_t : public node_t
	{
	public:
		lk_string value;
		literal_t(int line, const lk_string &s) : node_t(line), value(s) {  }
		virtual ~literal_t() {  }
	};

	class LKEXPORT null_t : public node_t
	{
	public:
		null_t( int line ) : node_t(line) {  }
		virtual ~null_t() {  }
	};

	void LKEXPORT pretty_print( lk_string &str, node_t *root, int level );
};

#endif
