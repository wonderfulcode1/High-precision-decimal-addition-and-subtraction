//时隔一个月看大工程。。。
//写完加减后感慨：好像小数整数本来就不应该分开QAQ
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;
struct list
{
	int data;
	list* next, * last;
};
struct bigint
{
	bool sign;//正负号,1正0,0负
	list* integerhead = NULL, * integerend = NULL;//整数
	list* decimalhead = NULL, * decimalend = NULL;//小数
	int intlen, declen;
	bigint()
	{
		integerhead = NULL; integerend = NULL;
		decimalhead = NULL; decimalend = NULL;
		intlen = 0; declen = 0; sign = 1;
	}
	bigint(string inte, string dec)//创建链表
	{
		if (inte[0] == '-')sign = 0;
		else sign = 1;
		intlen = inte.size() - 1 + sign; declen = dec.size();//从1开始
		list* p = new list;
		integerhead = p;
		p->last = NULL;
		for (int i = inte.size() - 1; i >= 1 - sign; i--)
		{
			p->data = inte[i] - '0';
			if (i != 1 - sign) { list* q1 = p; p = p->next = new list; p->last = q1; }
			else p->next = NULL;//边走边创建，最后NULL
		}integerend = p;
		/*while (p)
		{
			cout << p->data << " ";
			p = p->last;
		}cout << endl;*/
		list* q = new list;
		decimalhead = q;
		q->last = NULL;
		for (int i = 0; i < dec.size(); i++)
		{
			q->data = dec[i] - '0';
			if (i != dec.size() - 1) { list* p1 = q; q = q->next = new list; q->last = p1; }
			else q->next = NULL;
		}decimalend = q;
		/*while (q)
		{
			cout << q->data << " ";
			q = q->last;
		}cout << endl;*/
	}
	int decflatten()
	{
		list* p = decimalend;
		while (p->last)
		{
			p->last->data += p->data / 10;
			p->data %= 10;
			p = p->last;
		}
		int c = p->data / 10;
		p->data %= 10;
		return c;
	}
	void intflatten()
	{
		list* p = integerhead;
		while (p->next)
		{
			p->next->data += p->data / 10;
			p->data %= 10;
			p = p->next;
		}
		int f = p->data / 10;
		p->data %= 10;
		if (f) {
			p->next = new list;
			p->next->last = p;
			p = p->next;
			p->data = f;
			p->next = NULL;
			integerend = p;
			intlen++;
		}
	}
	void output(bool s)
	{
		list* p = integerend, * q = decimalhead;
		cout <<endl<< "= ";//(题目没要求)
		if ((!sign && s) || (sign && !s))cout << "-";
		int a = intlen, b = -intlen % 3;
		while (p)
		{
			if (b % 3 == 0 && a != intlen && a != 0)cout << ",";
			cout << p->data;
			p = p->last;
			a--; b++;
		}
		if (declen == 1 && q->data == 0);
		else {
			cout << ".";
			while (q)
			{
				cout << q->data;
				q = q->next;
			}
		}
		cout << endl<<endl;
		//cout << intlen<<" " << declen << endl;
	}
	void delete0()
	{
		list* p = integerend, * q = decimalend;
		while (p->data == 0 && intlen > 1)
		{
			list* w = p;
			p = p->last;
			delete w;
			p->next = NULL;
			intlen--;
		}integerend = p;
		while (q->data == 0 && declen > 1)
		{
			list* w = q;
			q = q->last;
			delete w;
			q->next = NULL;
			declen--;
		}decimalend = q;
	}
	void clear()
	{
		list* p = integerhead, * q = decimalhead;
		while (p)
		{
			list* w = p;
			p = p->next;
			delete w;
		}
		while (q)
		{
			list* w = q;
			q = q->next;
			delete w;
		}
		integerhead = NULL; integerend = NULL;
		decimalhead = NULL; decimalend = NULL;
		intlen = 0; declen = 0; sign = 1;
	}
};//
//到底会不会释放空间？
bool operator<(bigint X, bigint Y)//比大小，重载小于号
{
	bool flag = 1;
	if (X.intlen < Y.intlen)return 1;
	if (X.intlen > Y.intlen)return 0;//剩下等于
	list* p = X.integerend, * q = Y.integerend;
	while (p->data == q->data)//为什么p!=NULL不能做条件？
	{
		p = p->last;
		q = q->last;
		if (p == NULL) { flag = 0; break; }
	}
	if (flag)return p->data < q->data;
	if (!flag)
	{
		p = X.decimalhead; q = Y.decimalhead;
		while (p->data == q->data)//p==NULL又不让作条件
		{
			p = p->next;
			q = q->next;
			if (p == NULL && q == NULL)return 0;
			if (p == NULL && q != NULL)return 1;
			if (p != NULL && q == NULL)return 0;
		}
		return p->data < q->data;
	}
}
bigint operator-(bigint X, bigint Y)//先做大减小
{
	bigint Z;
	if (X < Y)
	{
		bigint c = X; X = Y; Y = c;
		Z.sign = 0;
	}//X>Y
	Z.declen = max(X.declen, Y.declen);
	Z.decimalend = new list;
	list* p = X.decimalend, * q = Y.decimalend, * r = Z.decimalend;
	r->next = NULL;
	int borrow = 0;
	if (X.declen < Y.declen)//有问题
	{
		int sum = Y.declen, difference = Y.declen - X.declen;
		while (difference)//这里???????????????
		{
			if (0 < borrow + q->data)
			{
				r->data = 10 - borrow - q->data;
				borrow = 1;
			}
			else
			{
				r->data = 0 - q->data - borrow;
				borrow = 0;
			}
			r->last = new list; list* w = r; r = r->last; r->next = w; q = q->last;//重写一边就对了？
			sum--; difference--;
		}
		while (sum)
		{
			if (p->data < q->data + borrow)
			{
				r->data = p->data - q->data + 10 - borrow;
				borrow = 1;
			}
			else {
				r->data = p->data - q->data - borrow;
				borrow = 0;
			}
			if (sum == 1)r->last = NULL;
			else { r->last = new list; r->last->next = r; r = r->last; p = p->last; q = q->last; }
			sum--;
		}
	}
	else//>=?长减小
	{
		int sum = X.declen, difference = X.declen - Y.declen;
		while (difference)
		{
			r->data = p->data;
			if (sum == 1)r->last = NULL;
			else { r->last = new list; r->last->next = r; r = r->last; }//p = p->last;q=q->last(复制粘贴时没删干净，出错)
			difference--; sum--; p = p->last;
		}
		borrow = 0;//下一位,借位
		while (sum)
		{
			if (p->data < q->data + borrow)
			{
				r->data = p->data - q->data + 10 - borrow;
				borrow = 1;
			}
			else {
				r->data = p->data - q->data - borrow;
				borrow = 0;
			}
			if (sum == 1)r->last = NULL;
			else { r->last = new list; r->last->next = r; r = r->last; p = p->last; q = q->last; }
			sum--;
		}
	}
	Z.decimalhead = r;//
	Z.integerhead = new list;
	Z.intlen = max(X.intlen, Y.intlen);
	p = X.integerhead; q = Y.integerhead; r = Z.integerhead;
	r->last = NULL;
	int sum = X.intlen, difference = Y.intlen;
	while (difference)
	{
		if (p->data < q->data + borrow)
		{
			r->data = p->data - q->data + 10 - borrow;
			borrow = 1;
		}
		else {
			r->data = p->data - q->data - borrow;
			borrow = 0;
		}
		if (sum == 1)r->next = NULL;
		else { r->next = new list; r->next->last = r; r = r->next; p = p->next; q = q->next; }
		sum--; difference--;
	}
	while (sum)
	{
		if (p->data < borrow)
		{
			r->data = p->data - borrow + 10;
			borrow = 1;
		}
		else
		{
			r->data = p->data - borrow;
			borrow = 0;
		}
		if (sum == 1)r->next = NULL;
		else { r->next = new list; r->next->last = r; r = r->next; p = p->next; }
		sum--;
	}
	Z.integerend = r;
	Z.delete0();
	return Z;
}
bigint operator+(bigint X, bigint Y)//带负号？
{
	bigint C;//其他情况同号
	C.decimalend = new list;

	C.declen = max(X.declen, Y.declen);
	list* p = X.decimalend, * q = Y.decimalend, * r = C.decimalend;
	r->next = NULL;
	if (X.declen >= Y.declen)//以x为首
	{
		int difference = X.declen - Y.declen;
		int sum = X.declen; C.declen = X.declen;
		while (sum)
		{
			if (difference)
			{
				r->data = p->data;
				p = p->last;
				r->last = new list; list* w = r; r = r->last; r->next = w;//构建链表
				difference--; sum--;
			}
			else
			{
				r->data = q->data + p->data;
				p = p->last; q = q->last;
				r->last = new list; list* w = r; r = r->last; r->next = w;
				sum--;
			}
		}
	}
	else
	{
		int difference = Y.declen - X.declen;
		int sum = Y.declen; C.declen = Y.declen;
		while (sum)
		{
			if (difference)
			{
				r->data = q->data;
				q = q->last;
				r->last = new list; list* w = r; r = r->last; r->next = w;//构建链表
				difference--; sum--;
			}
			else
			{
				r->data = q->data + p->data;
				p = p->last; q = q->last;
				r->last = new list; list* w = r; r = r->last; r->next = w;
				sum--;
			}
		}
	}list* z = r; r = r->next; delete z; z = NULL; r->last = NULL; C.decimalhead = r;
	int o = C.decflatten();
	C.integerhead = new list;
	p = X.integerhead; q = Y.integerhead; r = C.integerhead;
	r->last = NULL;
	if (X.intlen >= Y.intlen)
	{
		int sum = X.intlen, difference = Y.intlen; C.intlen = X.intlen;
		p->data += o;//此处改变了X头部值
		while (sum)
		{
			if (difference)
			{
				r->data = q->data + p->data;
				r->next = new list; list* w = r; r = r->next; r->last = w;
				q = q->next; p = p->next;
				difference--; sum--;
			}
			else
			{
				r->data = p->data;
				r->next = new list; list* w = r; r = r->next; r->last = w;
				p = p->next;
				sum--;
			}
		}
		X.integerhead->data -= o;
	}
	else
	{
		int sum = Y.intlen, difference = X.intlen; C.intlen = Y.intlen;
		q->data += o;//此处改变了Y头部值
		while (sum)
		{
			if (difference)
			{
				r->data = q->data + p->data;
				r->next = new list; list* w = r; r = r->next; r->last = w;
				q = q->next; p = p->next;
				difference--; sum--;
			}
			else
			{
				r->data = q->data;
				r->next = new list; list* w = r; r = r->next; r->last = w;
				q = q->next;
				sum--;
			}
		}
		Y.integerhead->data -= o;
	}z = r; r = r->last; delete z; z = NULL; r->next = NULL; C.integerend = r;
	C.intflatten();
	C.delete0();
	return C;//为什么返回的C不能输出，只能在函数里输出？(重载等于号的锅。。。)
}
bigint operator*(bigint X, bigint Y)
{
	bigint Z;
	Z.declen = X.declen + Y.declen;
	return Z;
}
bigint operator/(bigint X, bigint Y)
{
	return X;
}
int divide(int& flag, int& s, string c)
{
	if (c[0] == '-')s = -1;
	else s = 1;
	for (int i = 1; i < c.size(); i++)//1,一定不是运算符
	{
		switch (c[i])
		{
		case '+':flag = 1; return i;
		case '-':flag = 2; return i;
		case '*':flag = 3; return i;
		case '/':flag = 4; return i;
		case '<':flag = 5; return i;
		}
	}
}
int divide2(string& c)
{
	int i;
	for (i = 0; i < c.size(); i++)
		if (c[i] == '.')
			return i;
	c += ".0";
	return i;
}
void deletepoint(string& a)
{
	for (int i = 0; i < a.size(); i++)
	{
		if (a[i] == ',')
		{
			a.erase(a.begin() + i);
			i--;
		}
	}
}
void pleaseinput()//旧案，本来想直接输入算式会更人性化一点，为了过OJ就改了
{
	cout << "请输入两个数进行运算" << endl;
	cout << "支持整数,小数,半支持负数" << endl;
	cout << "计算：支持加减法,第二个数不能添多余的负号" << endl;
	cout << "示例如下：" << endl;
	cout << "-123.25134+73834.328 (回车)" << endl << "-123-437.29（回车）" << endl;
	cout << "请输入：" << endl;
}
//第二个数不支持负号
int main()
{
	pleaseinput();
	string a, b, c, a1, a2, b1, b2;
	while (1)
	{
		cout << " ";
		cin >> c;
		deletepoint(c);
		int flag, option, s;
		option = divide(flag, s, c);
		a = c.substr(0, option);
		b = c.substr(option + 1);
		c.clear();
		int optiona = divide2(a), optionb = divide2(b);
		a1 = a.substr(0, optiona);
		a2 = a.substr(1 + optiona);
		b1 = b.substr(0, optionb);
		b2 = b.substr(optionb + 1);
		bigint A(a1, a2), B(b1, b2);
		switch (flag * s)
		{
		case 1: { bigint c; c = A + B; c.output(1); break; }
		case 2: { bigint c; c = A - B; c.output(1); break; }
		case -2: { bigint c; c = A + B; c.output(0); break; }
		case -1: { bigint c; c = B - A; c.output(1); break; }
		case 3:cout << "无A * B功能"; break;
		case 4:cout << " 无A / B功能"; break;
		case 5: { bool i = A < B; cout << i << endl; break; }
		}
	}
}