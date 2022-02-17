#include "graphedge.h"
#include "graphnode.h"

GraphNode::GraphNode(int id)
{
    _id = id;
}

GraphNode::~GraphNode()
{
    //// STUDENT CODE
    ////
    // Task 5 : Moving the ChatBot -> Make all required changes in files chatlogic.h / chatlogic.cpp and graphnode.h / graphnode.cpp.
    // Nothing To do destructor shall be handled by smart pointer to dealocate the space
    // delete _chatBot; 

    ////
    //// EOF STUDENT CODE
}

void GraphNode::AddToken(std::string token)
{
    _answers.push_back(token);
}

void GraphNode::AddEdgeToParentNode(GraphEdge *edge)
{
    _parentEdges.push_back(edge);
}
// update function argument to be unique pointer 
// because each edge shall direct (point to) single node as a distination
void GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge)
{
    // Task 4 : Moving Smart Pointers
    // move the edge to child edges list to track edges pointing to this child
    _childEdges.push_back(std::move(edge));
}

//// STUDENT CODE
////
void GraphNode::MoveChatbotHere(ChatBot chatbot)
{
    // Task 5 : Moving the ChatBot
    _chatBot = std::move(chatbot);
    _chatBot.SetCurrentNode(this); //replacing -> with .
}

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode)
{
    newNode->MoveChatbotHere(std::move(_chatBot));
    // using move shall handle invalidating the pointer 
    // so no need to asign null pointer to it here
    // _chatBot = nullptr; // invalidate pointer at source
}
////
//// EOF STUDENT CODE

GraphEdge *GraphNode::GetChildEdgeAtIndex(int index)
{
    //// STUDENT CODE
    ////
    // return graph edges of specific child 
    // ss(accessing smart pointers value is done through .get())
    return _childEdges[index].get();

    ////
    //// EOF STUDENT CODE
}