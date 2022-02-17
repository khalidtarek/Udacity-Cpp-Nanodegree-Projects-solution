#ifndef CHATBOT_H_
#define CHATBOT_H_

#include <wx/bitmap.h>
#include <string>

class GraphNode; // forward declaration
class ChatLogic; // forward declaration

class ChatBot
{
private:
    // data handles (owned)
    wxBitmap *_image; // avatar image

    // data handles (not owned)
    GraphNode *_currentNode;
    GraphNode *_rootNode;
    ChatLogic *_chatLogic;

    // proprietary functions
    int ComputeLevenshteinDistance(std::string s1, std::string s2);

public:
    // constructors / destructors
    ChatBot();                     // constructor WITHOUT memory allocation
    ChatBot(std::string filename); // constructor WITH memory allocation
    // Task 2 : The Rule Of Five
    // The Rule Of Five -> destructor
    ~ChatBot();

    //// STUDENT CODE
    ////

    // Task 2 : The Rule Of Five
    // The Rule Of Five -> Move constructor
    ChatBot(ChatBot &&ChatBot);
    // The Rule Of Five -> Move Assign operator
    ChatBot& operator=(ChatBot&& ChatBot);
    // The Rule Of Five -> Copy constructor
    ChatBot(const ChatBot& ChatBot);
    // The Rule Of Five -> Copy asignment operator
    ChatBot& operator=(const ChatBot &ChatBot) ;
    
    ////
    //// EOF STUDENT CODE

    // getters / setters
    void SetCurrentNode(GraphNode *node);
    void SetRootNode(GraphNode *rootNode) { _rootNode = rootNode; }
    void SetChatLogicHandle(ChatLogic *chatLogic) { _chatLogic = chatLogic; }
    // Task 4 : Moving Smart Pointers
    ChatLogic* GetChatLogicHandle() { return _chatLogic; } 
    //std::move(_chatLogic); }
    wxBitmap *GetImageHandle() { return _image; }

    // communication
    void ReceiveMessageFromUser(std::string message);
};

#endif /* CHATBOT_H_ */
