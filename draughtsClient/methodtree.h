#ifndef METHODTREE_H
#define METHODTREE_H
#include <QPoint>
#include <QList>
class methodTree
{
public:
    QList<methodTree*> *child;
    QPoint p;
    int lenth;
    void getl();
    bool killing=0;
    methodTree(int x,int y);
    methodTree(int x,int y,int k);
    ~methodTree();
};
#endif // METHODTREE_H
