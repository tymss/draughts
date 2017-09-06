#include "methodtree.h"
#include <QDebug>
methodTree::methodTree(int x,int y)
{
    lenth=0;
    p.setX(x);
    p.setY(y);
    child=new QList<methodTree*>;
}
methodTree::methodTree(int x, int y, int k)
{
    lenth=0;
    p.setX(x);
    p.setY(y);
    killing=1;
    child=new QList<methodTree*>;
}
methodTree::~methodTree()
{
    for(int i=0;i<child->size();i++)
        delete child->at(i);
    delete child;
}

void methodTree::getl()
{
    int Max=0;
    QList<int> n;
    QList<methodTree*> tr;
    for(int i=0;i<child->size();i++)
    {
        child->at(i)->getl();
        if(Max<=child->at(i)->lenth)
        {
            Max=child->at(i)->lenth;
            if(n.isEmpty())
            {
                n<<i;
                tr<<child->at(i);
            }
            else if(child->at(n.last())->lenth<Max||child->at(n.last())->killing<child->at(i)->killing)
            {
                n.clear();
                n<<i;
                tr.clear();
                tr<<child->at(i);
            }
            else if(child->at(n.last())->lenth==Max&&child->at(n.last())->killing==child->at(i)->killing)
            {
                n<<i;
                tr<<child->at(i);
            }
        }
    }
    if(child->isEmpty())
        lenth=0;
    else
        lenth=Max+1;
    for(int i=0;i<child->size();i++)
    {
        if(n.indexOf(i)==-1)
            delete child->at(i);
    }
    child->clear();
    for(int j=0;j<tr.size();j++)
        child->append(tr.at(j));
}
