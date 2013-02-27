// Suffix tree.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include<iostream>
#include<string>
#include<fstream>
#include <vector>
using namespace std;
int piece=4000;
struct TrieEdge
{
	long from,to, k,p;
TrieEdge()
{
	from=-1;
	to=-1;
	k=-1;
	p=-1;
}
TrieEdge (long f,long s,long e,long t)
{
	from =f;
	to=t;
	k=s;
	p=e;
}
};
struct TrieNode
{
	long state;
	TrieEdge tr;
	long pos;
TrieNode ()
{
	state=-1;
	pos=-1;
}
TrieNode(long s,TrieEdge t,long p=-1)
{
	state =s;
	tr=TrieEdge (t.from,t.k,t.p,t.to);
	pos=p;
}

};
struct tns
{
	bool flag;
	TrieNode v;
	tns()
	{

	}
	tns(TrieNode ver,bool f):flag(f),v(ver)
	{}
};

class Trie
{
	struct ParentCont
	{
		long* parent;
		ParentCont()
		{
			parent=NULL;
		}
		ParentCont(long* p)
		{
			parent=p;
		}
		ParentCont& operator=(long* p)
		{
			parent=p;
			return *this;
		}
		~ParentCont()
		{
		if(parent!=NULL)	delete parent;
		}
		long& at(int i)
		{
			return parent[i];
		}
	};
	struct TrieEdgeCont
	{
		TrieEdge** e;
		TrieEdgeCont()
		{
			e=NULL;
		}
		TrieEdgeCont(TrieEdge** t)
		{
         e=t;
		}
		TrieEdgeCont(const TrieEdgeCont& t)
		{
			e=t.e;
		}

		TrieEdgeCont& operator=(TrieEdge** t)
		{
			e=t;
			return *this;
		}
		~TrieEdgeCont()
		{
		if(e!=NULL)
		{
			for(int i=0;i<piece;i++)
			 if(e[i]!=NULL)  delete e[i];
			delete e;
		}
		}
		TrieEdge& at(int i,int j)
		{
			return e[i][j];
		}
		TrieEdge*& at(int i)
		{
			return e[i];
		}
	};

	long count,initial;
	vector<ParentCont> parent;
	string str;
	vector<TrieEdgeCont> edges;
	void expand ();
	void replace(TrieEdge e);
	TrieNode& update(TrieNode v,long i);
	TrieNode& delta (TrieNode v,char a);
	TrieNode& Parent(TrieNode v);
	tns testAndSplit(TrieNode v,long i);
public:
	long getCount ()const
	{
		return count;
	}
	~Trie();
	Trie();
	Trie& add(char*nstr);
};
Trie::~Trie()
{
	for(int i=0;i<count%piece;i++)
		if(edges[count/piece].at(i)!=NULL)
			{
				delete edges[count/piece].at(i);
				edges[count/piece].at(i)=NULL;
			}

	for(int i=count%piece;i<piece;i++)
		edges[count/piece].at(i)=NULL;
}
Trie::Trie()
	{
		count=1;
		initial=0;
		parent.resize(7000);
		edges.resize(7000);
		long *parent1= new long[piece];
		parent1[0]=-2;
		parent[0]=parent1;
		TrieEdge** edges1=new TrieEdge*[piece];
		edges1[0]=new TrieEdge[6];
		edges[0]=edges1;
	}
Trie& Trie::add(char* nstr)
	{
		long l =str.length();
		TrieNode v,u(0,TrieEdge(-1,-1,-1,-1),-1);
		char a;
		str.append(nstr);
		a=str.at(l);
		v=delta( TrieNode(0,TrieEdge(-1,-1,-1,-1),-1) ,  a);
		while(v.state!=-1)
           {
			   l++;
			   if(edges[v.state/piece].at(v.state%piece)==NULL) break;
			   u=v;
			   v=delta(v,str.at(l));
			}
		for(long i=l;i<str.length();i++)
			u=update(u,i);
		return *this;
	}
TrieNode& Trie::Parent(TrieNode v)
{
	TrieEdge t (-1,-1,-1,-1);
	if(v.pos==-1) return TrieNode( parent[v.state/piece].at(v.state%piece),t,-1);
	  long	s=v.state , start=v.tr.k,end=v.pos-1;
	  if(s==initial)
	  {
		  start+=1;
		  if(start>end) return TrieNode (initial,TrieEdge(-1,-1,-1,-1),-1);
	  }
	else {s=parent[s/piece].at(s%piece);}
	while(true)
	{
		long i=0;
		while(str.at(start)!=str.at(edges[s/piece].at(s%piece,i).k))
		{
		i++;
		}
		TrieEdge edge2 = edges[s/piece].at(s%piece,i);
		if(end-start<edge2.p-edge2.k) return TrieNode(s,edge2,edge2.k+end-start+1);
		if(end-start == edge2.p-edge2.k) return TrieNode(edge2.to,TrieEdge(-1,-1,-1,-1),-1);
		s=edge2.to;start=start+edge2.p-edge2.k+1;
	}

}
TrieNode& Trie::delta(TrieNode v,char a)
{
	if(v.state==-1) return v;
	TrieEdge t = TrieEdge(-1,-1,-1,-1);
	TrieNode n = TrieNode(-1, t,-1);
	if(v.pos!=-1)
	{
		if(str.at(v.pos)!=a) return  n;
		else
		{
			long k=v.pos+1;
			if(k>v.tr.p) return TrieNode (v.tr.to,t,-1);
			else return TrieNode(v.state,v.tr,k);
		}
	}
	else
	{
		if(edges[v.state/piece].at(v.state%piece)==NULL) return TrieNode(-1,t,-1);
		long i=0;
		while(edges[v.state/piece].at(v.state%piece,i).k!=-1&&str.at(edges[v.state/piece].at(v.state%piece,i).k)!=a)
		i++;
		if(edges[v.state/piece].at(v.state%piece,i).k==-1) return TrieNode(-1,t,-1);
		TrieEdge e = edges[v.state/piece].at(v.state%piece,i);
		if(e.k+1>e.p) return TrieNode(e.to,t,-1);
		else return TrieNode(e.from,e,e.k+1);
	}
}
void Trie::expand()
{
	if(count/piece<7000)
	{
	TrieEdge** n1 = new TrieEdge*[piece];
	long *n3 = new long[piece];
	edges[count/piece]=n1;
	parent[count/piece]=n3;
	}
}
void Trie::replace(TrieEdge e)
{
	int i=0;
	while(i<6&&edges[e.from/piece].at(e.from%piece,i).k!=e.k&&edges[e.from/piece].at(e.from%piece,i).from!=-1) i++;
	if(i<6) edges[e.from/piece].at(e.from%piece,i)=e;
}
tns Trie::testAndSplit(TrieNode v,long i)
{
	TrieNode w = delta (v,str.at(i));
	if(w.state!=-1) return tns(v,true);
	if(v.pos==-1) return  tns(v,false);
	long z=count;
	if(count%piece==0) expand();
	TrieEdge e1 (v.state,v.tr.k,v.pos-1,z);
    edges[z/piece].at(z%piece)=new TrieEdge[6];
	replace(e1);
	TrieNode n1 (z,TrieEdge(-1,-1,-1,-1),-1);
	TrieEdge e2 (z,v.pos,v.tr.p,v.tr.to);
	replace(e2);
	count++;
	return tns(n1,false);
}
TrieNode& Trie::update(TrieNode v,long i)
{
	long prev=initial;
	TrieNode u=v;
	tns answ;
	answ.flag=false;
	while(answ.flag==false)
	{
		answ = testAndSplit(u,i);
		u=answ.v;
		if(!answ.flag)
		{
			long w =count;
			if(count%piece==0) expand();
			edges[w/piece].at(w%piece)=NULL;
			replace (TrieEdge(u.state,i,str.length()-1,w));
			count++;
			if(prev!=initial) parent[prev/piece].at(prev%piece) = u.state;
		    if(u.tr.from==-1&&u.state==initial){return u;}
			prev=u.state;
		    u=Parent(v);
			v=u;
		}
	}

	if(prev!=initial){parent[prev/piece].at(prev%piece)=u.state;}
	return delta(u,str.at(i));
}
int _tmain(int argc, _TCHAR* argv[])
{
Trie a;
char mas[102];
//ofstream b(argv[1]);
//int l;
//for(int i=1;i<=150000;i++)
//{
//	l=101;
//	for(int j=0;j<l-2;j++)
//		mas[j]=abs(rand()%5)+66;
//	mas[l-2]='$';
//	mas[l-1]='\0';
//	b<<mas;
//    if(i==150000) break;
//	if(i%10==0 )b<<endl;
//	else b<<' ';
//}
//b.close();
ifstream f("new2.txt");
int i=0;
while(!f.eof()&&i<100)
{
	i++;
	f.getline(mas,101);
	int l=strlen(mas);
	if(strcmp(mas,"")==0) cout<<"Correct assumption!";
	cout<<mas<<endl;;
	//a.add(mas);
}
cout<<i;
f.close();
cout<<a.getCount();
}

