#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <random>
#define TRUE 1
#define FALSE 0
using namespace std;


unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine e(seed);

class Coordinate
{
public:
  Coordinate( Coordinate *);
  Coordinate( double, double );
  Coordinate(){x=0;y=0;};
  void set(double, double);
  void set(Coordinate *);
  double getx(){return x;};
  double gety(){return y;};
  double distancefrom(Coordinate*);
  double distancefrom(double,double);
  friend ostream &operator << (ostream &out, const Coordinate &c); 
  friend istream &operator >> (istream &input, Coordinate &c );

  bool operator==(const Coordinate& c) const{return ( (c.x == x) && (c.y == y)) ;}
  ~Coordinate();
private:
  double x;
  double y;
};
ostream & operator << (ostream &out, const Coordinate &c) 
{
  out << "(" << c.x << "," << c.y << ")"; 
  return out; 
}

istream & operator >> (istream &input, Coordinate &c)
{ 
  input >> c.x >> c.y;
  return input;            
}

Coordinate::~Coordinate()
{
#ifdef DEBUG
  cerr << "Deleteing coordinate " << *this << endl;
#endif
}
void Coordinate::set( Coordinate * c )
{
  x=c->getx();
  y=c->gety();
  return;
}

Coordinate::Coordinate( double x, double y )
{
#ifdef DEBUG
  cerr << "[ Coordinate Constructor(" << x << "," << y << ") ]" << endl;
#endif
  this->x = x;
  this->y = y;

  return;
}
Coordinate::Coordinate( Coordinate * c )
{
#ifdef DEBUG
  cerr << "[ Coordinate Constructor(Coordinate * c) ]" << endl;
#endif
  
  this->x = c->getx();
  this->y = c->gety();

  return;
}

  
void Coordinate::set(double x, double y)
{
#ifdef DEBUG
  cerr << "[ Coordinate::set(double,double) ]" << endl;
#endif
  this->x = x;
  this->y = y;
  return;
}

double Coordinate::distancefrom(Coordinate* c)
{
#ifdef DEBUG
  cerr << "[calculate distance from " << *c << " to " << *this << "]" << endl;
#endif
  double x1 = c->getx();
  double y1 = c->gety();
  return sqrt( ( x1 - this->x )*( x1 - this->x ) + ( y1 - this->y)*( y1 - this->y) );
}

double Coordinate::distancefrom(double x1, double y1)
{
#ifdef DEBUG
  cerr << "[calculate distance from (" << x1 << "," << y1 << ") to " << *this << "]" << endl;
#endif
  return sqrt( ( x1 - this->x )*( x1 - this->x ) + ( y1 - this->y)*( y1 - this->y) );
}


class Link
{
public:
  Link( Coordinate *, Coordinate *);
  Coordinate * getCoordinate1(){ return c1; };
  Coordinate * getCoordinate2(){ return c2; };
  double distance(){ return c1->distancefrom(c2); };
  ~Link();
private:
  Link();
  Coordinate * c1;
  Coordinate * c2;
};

Link::Link()
{
#ifdef DEBUG
  cerr << "Default Link Constructor (private)" << endl;
#endif
}

Link::~Link()
{
#ifdef DEBUG
  cerr << "Link Destructor" << endl;
#endif  
  delete c1;
  delete c2;
}

Link::Link( Coordinate * A, Coordinate * B)
{
#ifdef DEBUG
  cerr << "new Link(" << *A << "," << *B << ")" << endl;
#endif  
  c1 = new Coordinate( A->getx(), A->gety() );
  c2 = new Coordinate( B->getx(), B->gety() );
}

class NeuralNode
{
public:
  NeuralNode(NeuralNode*);
  NeuralNode(double,double);
  NeuralNode(Coordinate*);
  ~NeuralNode();

  void dumpSVG();
  void dumpSVGNode();
  void dumpSVGLinks();
  void dumpSVGName();
  
  void addLink(NeuralNode*);
  void addOneWayLink(NeuralNode*);
  vector<NeuralNode*> getLinks(){ return pLinks; };
  void removeLink(NeuralNode*);
  
  int howManyLinks(){ return pLinks.size(); };

  string getName(){ return name; };
  void setName( string s ){ name = s;};

  Coordinate * getCoordinate(){ return pCoordinate; };
  void setCoordinate(double x, double y);
  void setCoordinate( Coordinate * c );
  friend ostream &operator << (ostream &out, const NeuralNode &n); 

  NeuralNode * getParent(){ return pParent; };
  void setParent( NeuralNode * N ){ pParent = N; };

  void shuffle(){ std::shuffle( pLinks.begin(), pLinks.end(), e ); };
private:
  NeuralNode(void);
  vector<NeuralNode*> pLinks;
  string name;
  Coordinate * pCoordinate;
  NeuralNode * pParent;
};

void NeuralNode::dumpSVGNode()
{
  cout << "<circle cx=\"" << pCoordinate->getx() << "\" cy=\"" << pCoordinate->gety() << "\" r=\"5\" stroke=\"black\" fill=\"white\" stroke-width=\"1\"/>" << endl;
  return;
}

void NeuralNode::dumpSVGName()
{
  cout << "<text x=\"" << pCoordinate->getx()-4 << "\" y=\"" << pCoordinate->gety()+20 << "\" fill=\"blue\">" << name << "</text>" << endl;
  return;
}

void NeuralNode::dumpSVGLinks()
{
  for( int i=0; i<pLinks.size(); i++ )
    {
      cout << "<line x1=\"" << pCoordinate->getx() << "\" y1=\"" << pCoordinate->gety()<< "\" x2=\"" << pLinks[i]->getCoordinate()->getx() << "\" y2=\"" <<  pLinks[i]->getCoordinate()->gety() << "\" stroke=\"gray\" stroke-width=\".5\"/>" << endl;
    }
  return;
}



void NeuralNode::dumpSVG()
{
  for( int i=0; i<pLinks.size(); i++ )
    {
      cout << "<line x1=\"" << pCoordinate->getx() << "\" y1=\"" << pCoordinate->gety()<< "\" x2=\"" << pLinks[i]->getCoordinate()->getx() << "\" y2=\"" <<  pLinks[i]->getCoordinate()->gety() << "\" stroke=\"black\" stroke-width=\"1\"/>" << endl;
    }
  cout << "<circle cx=\"" << pCoordinate->getx() << "\" cy=\"" << pCoordinate->gety() << "\" r=\"5\" stroke=\"red\" fill=\"white\" stroke-width=\"1\"/>" << endl;
  

  return;
}

NeuralNode::~NeuralNode()
{
#ifdef DEBUG
  cerr << "Deleting Node: " << *pCoordinate << endl;
#endif
  delete pCoordinate;
}
void NeuralNode::removeLink( NeuralNode* N )
{
  
  Coordinate * tmpCoord = N->getCoordinate();
  for( int i=0; i<pLinks.size(); i++ )
    {
      if( *tmpCoord == *pLinks[i]->getCoordinate() )
	{
#ifdef DEBUG
	  cerr << "removing " << *pLinks[i] << " from " << name << endl;
#endif
	  pLinks.erase(pLinks.begin()+i);
	}
    }
  

}
void NeuralNode::addLink( NeuralNode* N )
{
  bool okaytoadd = TRUE;
  // if link already exists then don't add it
  for( int i=0; i<pLinks.size(); i++ )
    {
      if( pLinks[i]->getCoordinate() == N->getCoordinate() )
	{
	  cerr << "not okay to add link from " << *N << " to " << *pLinks[i] << " because one already exists" << endl;
	  okaytoadd = FALSE;
	}
    }
  if( *N->getCoordinate() == *pCoordinate )
    {
      cerr << "cannot link to one's self!" << endl;
    }
  else
    {
      if( okaytoadd )
	{
	  pLinks.push_back(N);
	  N->addOneWayLink(this);
	}
    }
  return;
}

void NeuralNode::addOneWayLink( NeuralNode* N )
{
  bool okaytoadd = TRUE;
  // if link already exists then don't add it
  for( int i=0; i<pLinks.size(); i++ )
    {
      if( pLinks[i]->getCoordinate() == N->getCoordinate() )
	{
	  cerr << "not okay to add this link because one already exists" << endl;
	  okaytoadd = FALSE;
	}
    }
    
  if( *N->getCoordinate() == *pCoordinate )
    {
      cerr << "cannot link to one's self!" << endl;
    }
  else
    {
      if( okaytoadd ) pLinks.push_back(N);
    }
  return;
}

NeuralNode::NeuralNode(double x, double y)
{
  pCoordinate=new Coordinate(x,y);
  pParent=NULL;
  return;
}
NeuralNode::NeuralNode(Coordinate * c)
{
  // make a copy of c -- this allows c to be deleted w/o modifying Neural Node
  if(pCoordinate != NULL) delete pCoordinate;
  pCoordinate=new Coordinate( c->getx(), c->gety() );
  pParent=NULL;
  return;
}
NeuralNode::NeuralNode()
{
  pCoordinate=new Coordinate( 0, 0 );
  pParent=NULL;
  return;
}
NeuralNode::NeuralNode(NeuralNode* N)
{
  if( pCoordinate == NULL )
    {
      pCoordinate = new Coordinate( N->getCoordinate() );
    }
  else
    {
      pCoordinate->set( N->getCoordinate() );
    }
  name = "";
  pParent = N->getParent();
  return;
}

void NeuralNode::setCoordinate( Coordinate * c )
{
  if( pCoordinate != NULL ) delete pCoordinate;
  pCoordinate = new Coordinate( c->getx(), c->gety() );
  // pCoordinate->setname( c->getName() );      
  return;
}

ostream & operator << (ostream &out, const NeuralNode &c) 
{
  out << *c.pCoordinate << " [ ";
  for( int i=0; i<c.pLinks.size(); i++ )
    {
      out << *c.pLinks[i]->getCoordinate();
    }
  out << " ] (" << c.name << ")"; 
  return out; 
}


class Path
{
public:
  Path(){};
  ~Path(){};
  double getLength(){ return length; };
  void setLength( double l ){ length=l; };
  vector<NeuralNode*> getPath(){ return pPath; };
  void addNode( NeuralNode* N){pPath.push_back(N);};
  void clear(){ pPath.clear();};
  int size(){ return pPath.size(); };
  NeuralNode* getNode(int i){ return pPath[i]; };
  void reverse(){ std::reverse( pPath.begin(), pPath.end());};
private:
  vector<NeuralNode*> pPath;
  double length;
};

  
  
  
  
vector<NeuralNode*> breadcrumbs;
vector<NeuralNode*> viewableworld;
vector<NeuralNode*> path;
Path * mypath;
double path_length=0;
int MODE=0;

void explore( NeuralNode * N)
{
#ifdef DEBUG
  cerr << "exploring: " << *N << " which has " << N->howManyLinks() << " links" << endl;
#endif
  breadcrumbs.push_back( N );
  
  if( viewableworld.size() == 0 ) viewableworld.push_back( N );
  
  // Explore Links from 0 to how Many There Are
  // *********************

  if( MODE == 0 )
    {
      for( int i=0; i< N->howManyLinks(); i++ )
	{
	  bool alreadyexplored=false;
	  
	  // check to make sure that N->getLinks[i] is not in breadcrumbs yet.
	  for( int j=0; j<breadcrumbs.size(); j++ )
	    {
	      if( *breadcrumbs[j]->getCoordinate() == *N->getLinks()[i]->getCoordinate() )
		{
#ifdef DEBUG
		  cerr << "found " << *N->getLinks()[i]->getCoordinate() << " in breadcrumbs - don't re-explore" << endl;
#endif
		  alreadyexplored=true;
		}
	    }
	  if( !alreadyexplored )
	    {
	      // add the new one to the viewable world list
	      N->getLinks()[i]->setParent(N);
	      //cerr << endl << *N << endl << endl;
	      viewableworld.push_back( N->getLinks()[i] );
	      explore( N->getLinks()[i] );
	    }
	}
    }
  else if (MODE==1 )
    //************************** EXPLORE MODE = 1;
    {
      for( int i=N->howManyLinks()-1; i>=0; i-- )
	{
	  bool alreadyexplored=false;
	  
	  // check to make sure that N->getLinks[i] is not in breadcrumbs yet.
	  for( int j=0; j<breadcrumbs.size(); j++ )
	    {
	      if( *breadcrumbs[j]->getCoordinate() == *N->getLinks()[i]->getCoordinate() )
		{
#ifdef DEBUG
		  cerr << "found " << *N->getLinks()[i]->getCoordinate() << " in breadcrumbs - don't re-explore" << endl;
#endif
		  alreadyexplored=true;
		}
	    }
	  if( !alreadyexplored )
	    {
	      // add the new one to the viewable world list
	      N->getLinks()[i]->setParent(N);
	      //cerr << endl << *N << endl << endl;
	      viewableworld.push_back( N->getLinks()[i] );
	      explore( N->getLinks()[i]);
	    }
	}
    }
  // ************************************
  else if ( MODE==2 )
    //************************** EXPLORE MODE = 2;
    {
            N->shuffle();

      for( int i=0; i< N->howManyLinks(); i++ )
	{
	  bool alreadyexplored=false;
	  
	  // check to make sure that N->getLinks[i] is not in breadcrumbs yet.
	  for( int j=0; j<breadcrumbs.size(); j++ )
	    {
	      if( *breadcrumbs[j]->getCoordinate() == *N->getLinks()[i]->getCoordinate() )
		{
#ifdef DEBUG
		  cerr << "found " << *N->getLinks()[i]->getCoordinate() << " in breadcrumbs - don't re-explore" << endl;
#endif
		  alreadyexplored=true;
		}
	    }
	  if( !alreadyexplored )
	    {
	      // add the new one to the viewable world list
	      N->getLinks()[i]->setParent(N);
	      viewableworld.push_back( N->getLinks()[i] );

	      explore( N->getLinks()[i] );
	    }
	}
    }
  // ************************************


  return;
}

bool reachable( NeuralNode * StartNode, NeuralNode * GoalNode )
{
#ifdef DEBUG
  cerr << "looking for: " << *GoalNode << " from " << *StartNode  <<  endl;
#endif
  bool retVal = false;
  explore( StartNode);
  for( int i=0; i<viewableworld.size(); i++ )
    {
      if(*viewableworld[i]->getCoordinate() == *GoalNode->getCoordinate())
	{
	  retVal = TRUE;
	}
    }
  return retVal;
}

Path *  getPath( NeuralNode * start, NeuralNode * goal)
{
  Path * retVal = new Path();
  retVal->clear();
  
  path_length=0;
  breadcrumbs.clear();
  path.clear();
  viewableworld.clear();
  if( reachable( start, goal ) )
    {
      NeuralNode * n = goal;
      Coordinate * a = n->getCoordinate();
      while( n != start )
	{
	  path.push_back( n );
	  n = n->getParent();
	  Coordinate * b = n->getCoordinate();
	  path_length+=a->distancefrom(b);
	  a=b;
	}
      path.push_back( n );
    }
  reverse( path.begin(), path.end() );
  for( int i=0; i<path.size(); i++ )
    {
      retVal->addNode( path[i] );

    }
  retVal->setLength( path_length);
  return retVal;
}

void dumpPath( Path p )
{
  vector<NeuralNode *> myPath = p.getPath();
  
  if( p.size() == 0 ) cerr << "NO ROUTE" << endl << endl;
  for( int i=0; i< p.size()-1; i++ )
    {
      cerr << /* *myPath[i]->getCoordinate()  << */ "(" << myPath[i]->getName() << ")->";
    }
  cerr << /* *myPath[p.size()-1]->getCoordinate() << */ "(" << myPath[p.size()-1]->getName() << ")" << endl;
}

// -------------------------------------------------------


int main()
{

  mypath=new Path();
  vector<NeuralNode*> nodes;

  // Map of Prospect Hill Street Intersections and bends
  // Beverly, Mass. 
  nodes.push_back( new NeuralNode(-20,-20));
  nodes.push_back( new NeuralNode(58,19));
  nodes.push_back( new NeuralNode(95,48));
  nodes.push_back( new NeuralNode(126,76));
  nodes.push_back( new NeuralNode(159,106));
  nodes.push_back( new NeuralNode(166,7));
  nodes.push_back( new NeuralNode(195,34));
  nodes.push_back( new NeuralNode(213,69));
  nodes.push_back( new NeuralNode(228,101));
  nodes.push_back( new NeuralNode(185,124));
  nodes.push_back( new NeuralNode(214,124));
  nodes.push_back( new NeuralNode(60,105));
  nodes.push_back( new NeuralNode(175,154));
  nodes.push_back( new NeuralNode(59,145));
  nodes.push_back( new NeuralNode(22,163));
  nodes.push_back( new NeuralNode(31,202));
  nodes.push_back( new NeuralNode(25,254));
  nodes.push_back( new NeuralNode(75,269));
  nodes.push_back( new NeuralNode(115,288));
  nodes.push_back( new NeuralNode(162,182));
  nodes.push_back( new NeuralNode(141,220));
  nodes.push_back( new NeuralNode(164,226));
  nodes.push_back( new NeuralNode(181,281));
  nodes.push_back( new NeuralNode(152,310));
  nodes.push_back( new NeuralNode(189,216));
  nodes.push_back( new NeuralNode(236,281));
  nodes.push_back( new NeuralNode(210,195));
  nodes.push_back( new NeuralNode(247,261));
  nodes.push_back( new NeuralNode(215,167));
  nodes.push_back( new NeuralNode(286,173));
  nodes.push_back( new NeuralNode(297,214));
  nodes.push_back( new NeuralNode(280,236));
  nodes.push_back( new NeuralNode(268,303));
  nodes.push_back( new NeuralNode(194,311));
  nodes.push_back( new NeuralNode(176,350));
  nodes.push_back( new NeuralNode(105,368));
  nodes.push_back( new NeuralNode(5,410));
  nodes.push_back( new NeuralNode(41,428));
  nodes.push_back( new NeuralNode(76,448));
  nodes.push_back( new NeuralNode(153,484));
  nodes.push_back( new NeuralNode(100,411));
  nodes.push_back( new NeuralNode(182,455));
  nodes.push_back( new NeuralNode(142,381));
  nodes.push_back( new NeuralNode(190,392));
  nodes.push_back( new NeuralNode(203,432));
  nodes.push_back( new NeuralNode(218,409));
  nodes.push_back( new NeuralNode(187,140));

  // nodes.push_back( new NeuralNode( 700, 700 ));
  //nodes[12]->addLink( nodes[47] );
  //nodes[47]->addLink( nodes[3] );



  
  // *******
  nodes[1]->addLink( nodes[2]);
  nodes[1]->addLink( nodes[11]);
  nodes[2]->addLink( nodes[3]);
  nodes[3]->addLink( nodes[4]);
  nodes[4]->addLink( nodes[7]);
  nodes[5]->addLink( nodes[6]);
  nodes[8]->addLink( nodes[10]);
  nodes[9]->addLink( nodes[10]);
  nodes[10]->addLink( nodes[28]);
  nodes[11]->addLink( nodes[12]);
  nodes[12]->addLink( nodes[19]);
  nodes[13]->addLink( nodes[14]);
  nodes[14]->addLink( nodes[15]);
  nodes[15]->addLink( nodes[16]);
  nodes[16]->addLink( nodes[17]);
  nodes[17]->addLink( nodes[18]);
  nodes[18]->addLink( nodes[20]);
  nodes[21]->addLink( nodes[22]);
  nodes[22]->addLink( nodes[23]);
  nodes[25]->addLink( nodes[27]);
  nodes[26]->addLink( nodes[28]);
  nodes[28]->addLink( nodes[31]);
  nodes[30]->addLink( nodes[31]);
  nodes[32]->addLink( nodes[45]);
  nodes[33]->addLink( nodes[34]);
  nodes[35]->addLink( nodes[42]);
  nodes[36]->addLink( nodes[37]);
  nodes[38]->addLink( nodes[39]);
  nodes[39]->addLink( nodes[41]);
  nodes[43]->addLink( nodes[45]);
  
  nodes[2]->addLink( nodes[5]);
  nodes[3]->addLink( nodes[6]);
  nodes[4]->addLink( nodes[9]);
  nodes[6]->addLink( nodes[7]);
  nodes[7]->addLink( nodes[8]);
  nodes[11]->addLink( nodes[13]);
  nodes[12]->addLink( nodes[46]);
  nodes[13]->addLink( nodes[19]);
  nodes[15]->addLink( nodes[20]);
  nodes[16]->addLink( nodes[36]);
  nodes[17]->addLink( nodes[37]);
  nodes[18]->addLink( nodes[23]);
  nodes[19]->addLink( nodes[24]);
  nodes[20]->addLink( nodes[21]);
  nodes[21]->addLink( nodes[24]);
  nodes[22]->addLink( nodes[25]);
  nodes[23]->addLink( nodes[34]);
  nodes[24]->addLink( nodes[26]);
  nodes[25]->addLink( nodes[32]);
  nodes[27]->addLink( nodes[31]);
  nodes[29]->addLink( nodes[30]);
  nodes[33]->addLink( nodes[43]);
  nodes[34]->addLink( nodes[42]);
  nodes[35]->addLink( nodes[40]);
  nodes[37]->addLink( nodes[38]);
  nodes[38]->addLink( nodes[40]);
  nodes[40]->addLink( nodes[41]);
  nodes[41]->addLink( nodes[44]);
  nodes[42]->addLink( nodes[44]);
  nodes[44]->addLink( nodes[45]);
  
  nodes[18]->addLink( nodes[35]);
  nodes[22]->addLink( nodes[33]);
  nodes[24]->addLink( nodes[27]);
 
  // *******

  nodes[1]->setName("1");
  nodes[2]->setName("2");
  nodes[3]->setName("3");
  nodes[4]->setName("4");
  nodes[5]->setName("5");
  nodes[6]->setName("6");
  nodes[7]->setName("7");
  nodes[8]->setName("8");
  nodes[9]->setName("9");
  nodes[10]->setName("10");
  nodes[11]->setName("11");
  nodes[12]->setName("12");
  nodes[13]->setName("13");
  nodes[14]->setName("14");
  nodes[15]->setName("15");
  nodes[16]->setName("16");
  nodes[17]->setName("17");
  nodes[18]->setName("18");
  nodes[19]->setName("19");
  nodes[20]->setName("20");
  nodes[21]->setName("21");
  nodes[22]->setName("22");
  nodes[23]->setName("23");
  nodes[24]->setName("24");
  nodes[25]->setName("25");
  nodes[26]->setName("26");
  nodes[27]->setName("27");
  nodes[28]->setName("28");
  nodes[29]->setName("29");
  nodes[30]->setName("30");
  nodes[31]->setName("31");
  nodes[32]->setName("32");
  nodes[33]->setName("33");
  nodes[34]->setName("34");
  nodes[35]->setName("35");
  nodes[36]->setName("36");
  nodes[37]->setName("37");
  nodes[38]->setName("38");
  nodes[39]->setName("39");
  nodes[40]->setName("40");
  nodes[41]->setName("41");
  nodes[42]->setName("42");
  nodes[43]->setName("43");
  nodes[44]->setName("44");
  nodes[45]->setName("45");
  nodes[46]->setName("46");
  

  
  //nodes[]->addLink( nodes[] );

  int start_node = 1;
  int goal_node = 18;
    cerr << "Mode: First First" << endl;

  MODE=0;
 Path p = *getPath( nodes[start_node], nodes[goal_node] );   
 dumpPath(p);
 cerr << "Path 1 distance travelled: " << p.getLength() << endl << endl;

 MODE=1;

  cerr << "Mode: First Last" << endl;

 Path q =  *getPath( nodes[start_node], nodes[goal_node] );   
 dumpPath(q);
 cerr << "Path 2 distance travelled: " << q.getLength() << endl << endl;
 
 double minimum_length = 0;
 int minimum_attempt = 0;
 


 MODE=2;
 cerr << "Mode: Brute Force" << endl;
 vector<Path> paths;
 for( int i=0; i<500; i++ )
   {
     paths.push_back( *getPath( nodes[start_node], nodes[goal_node] ) );
     if( minimum_length==0 ) minimum_length = paths[i].getLength();
     if( paths[i].getLength() < minimum_length )
       {
	 minimum_length = paths[i].getLength();
	 minimum_attempt = i;
       };
     
   }

 dumpPath( paths[minimum_attempt] );
 cerr << "Path 3 distance travelled: " << minimum_length << endl << "Attempt #: " << minimum_attempt << endl;

 cerr << endl << endl << endl;

 double minx = 0;
 double maxx = 0;
 double miny = 0;
 double maxy = 0;
 
 // go through and get min and max x's and y's
 for( int i=0; i<nodes.size(); i++ )
   {
     if( nodes[i]->getCoordinate()->getx() > maxx )
       {
	 maxx = nodes[i]->getCoordinate()->getx();
       }

     if( nodes[i]->getCoordinate()->getx() < minx )
       {
	 minx = nodes[i]->getCoordinate()->getx();
       }
     
     if( nodes[i]->getCoordinate()->gety() > maxy )
       {
	 maxy = nodes[i]->getCoordinate()->gety();
       }

     if( nodes[i]->getCoordinate()->gety() < miny )
       {
	 miny = nodes[i]->getCoordinate()->gety();
       }

     maxx+=10;
     maxy+=10;
     
   }


 cout << "<html><head><title>My Neural Network</title></head><body>" << endl;
 cout << "<svg width=\"" << maxx << "px\" height=\"" << maxy << "px\" version=\"1.1\"><defs></defs>" << endl;

 for( int i=0; i<nodes.size(); i++ )
   {
     nodes[i]->dumpSVGLinks();
   }

 vector <NeuralNode*>  minpath = paths[minimum_attempt].getPath();
 //vector <NeuralNode*>  minpath = q.getPath();
 //vector <NeuralNode*>  minpath = r.getPath();
 
for( int i=0; i<minpath.size()-1; i++ )
   {
     double myx1=minpath[i]->getCoordinate()->getx();
     double myy1=minpath[i]->getCoordinate()->gety();
     double myx2=minpath[i+1]->getCoordinate()->getx();
     double myy2=minpath[i+1]->getCoordinate()->gety();
     
     cout << "<line x1=\"" << myx1 << "\" y1=\"" << myy1 << "\" x2=\"" << myx2 << "\" y2=\"" << myy2 << "\" stroke=\"green\" stroke-width=\"3\"/>" << endl;

        }


 for( int i=0; i<nodes.size(); i++ )
   {
     nodes[i]->dumpSVGNode();
   }
 for( int i=0; i<nodes.size(); i++ )
   {
     nodes[i]->dumpSVGName();
   }

 cout << "</svg></body></html>" << endl;
 
 

 return( 0 );
}
