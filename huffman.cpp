// Name: Brian Neas
// Student ID: C282H554
// Assignment: 8

// !!!! ADD Zero's back in!!!

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

// node class to store data of the letter, its frequency, and children nodes
struct Node {
   char letter;
   int frequency; // how often the letter appears
   Node* left; // left node
   Node* right; // right node

   Node() : letter(0), frequency(0), left(NULL), right(NULL) { }
   Node(char letter) : letter(letter), frequency(0), left(NULL), right(NULL) { }
   Node(char letter, int frequency) : letter(letter), frequency(frequency),
      left(NULL), right(NULL) { }
   Node(char letter, int frequency, Node* left, Node* right) : letter(letter),
      frequency(frequency), left(left), right(right) { }

   // increase frequency when called (a letter is found)
   void iterate() {
      frequency++;
   }

   // print the letter with its frequency
   void print() {
      cout << letter << ": " << frequency << endl;
   }
};

// imports data from the specified text file and determines the letter frequency
// of each of the letters in the vector
void import(vector<Node*>& letters) {
   // open fil and check if it failed
   ifstream file;
   file.open("doc.txt");

   if (!file) {
      cerr << "doc.txt could not be opened." << endl;
      exit(0);
   }

   // import all of the data from the text file
   while (!(file.eof())) {
      char nextLetter;
      file >> nextLetter; // get next letter from file
      nextLetter = tolower(nextLetter); // make the letter lowercase

      // check if the letter is punctuation, if not, then iterate its frequency
      if (!ispunct(nextLetter)) {
         for(vector<Node*>::iterator it = letters.begin();
            it != letters.end(); ++it) {
            if (nextLetter == (*it)->letter) {
               // increase frequency by 1 since letter was found
               (*it)->iterate();
            }
         }
      }
   }

   file.close();
}

// populates vector with letter nodes, a-z
void populateVector(vector<Node*>& letters) {
   for (char l = 'a'; l <= 'z'; l++) {
      Node* newLetter = new Node(l);
      letters.push_back(newLetter);
   }
}

// iterates through the vector and finds the letter object with the lowest
// frequency, then returns it
Node* findSmallestNode(vector<Node*>& letters) {
   Node* smallestNode = letters[0]; // start with first letter
   // get iterator pointer to the first element in the vector
   vector<Node*>::iterator smallestNodePosition = letters.begin();

   // iterate over the vector and find the loqest frequency letter
   for(vector<Node*>::iterator it = letters.begin(); it != letters.end();
      ++it) {

      // if the frequency is lower, set temp to that lowest value
      if ((*it)->frequency < smallestNode->frequency) {
         smallestNode = *it; // get copy of the smallest node
         smallestNodePosition = it; // get pointer to that node
      }
   }

   // remove that pointer from the vector
   letters.erase(smallestNodePosition);

   return smallestNode;
}

// builds the huffman (binary) tree by finding the two lowest frequency nodes,
// summing them, and then creating a parent node above them to store their
// summed frequency. it is then added back into the vector to be used on the
// next round. this will continue until there is only one "root" node left
Node* buildHuffmanTree(vector<Node*>& letters) {
   while (letters.size() > 1) {
      // get the smallest node for the left and smallest for the right
      // note: these are now removed from the vector of nodes
      Node* leftNode = findSmallestNode(letters);
      Node* rightNode = findSmallestNode(letters);

      // sum their frequencies and create a new parent node with that sum
      int sumOfFreq = leftNode->frequency + rightNode->frequency;
      Node* sumNode = new Node('-', sumOfFreq, leftNode, rightNode);

      // print out the new tree created
      cout << "Parent Node: " << sumNode->letter << " Value: " << sumNode->frequency << endl;
      cout << "Left Node: " << leftNode->letter << " Value: " << leftNode->frequency << endl;
      cout << "Right Node: " << rightNode->letter << " Value: " << rightNode->frequency << endl;
      cout << "--------" << endl;

      letters.push_back(sumNode); // push that new node to the vector
   }

   // only one node left in letters vector, which will now become your root
   Node* root = letters[0];
   return root;
}

// checks to see if there are any letters with a frequency of zero left in the
// letters vector
bool zeroFreqPresent(vector<Node*> letters) {
   for (int i = 0; i < letters.size(); i++) {
      if (letters[i]->frequency == 0) { // check frequency of each letter
         return true;
      }
   }

   return false;
}

// removes any zero frequency letters (nodes) in the letters vector
void removeZeroEntries(vector<Node*>& letters) {
   while(zeroFreqPresent(letters)) { // check for zero frequency letters
      int position = 0;

      // get position of the next zero frequency node
      for (int i = 0; i < letters.size(); i++){
         if (letters[i]->frequency == 0) {
            position = i;
         }
      }

      // release the allocated memory for the letter
      delete letters[position];

      // erase zero entry at that position in the vector
      letters.erase((letters.begin() + position));
   }
}

// uses in order binary tree traversal to determine the huffman coding for each
// letter
void inOrderCoding(Node* root, vector<int>& code) {
   if (root) { // check for null node
      // before going to left node, push a 0 indicating a left traversal
      code.push_back(0);
      inOrderCoding(root->left, code); // call on left node

      // ready to print, since all left-most nodes have been traversed
      // check if the letter does not equal a "sum" node, and if it doesn't
      // print out the letter and its code which is stored in the code vector
      if (root->letter != '-') {
         cout << root->letter << ": "; // print letter

         // entire code stored in vector
         for (int i = 0; i < code.size(); i++) {
            cout << code[i];
         }

         cout << "\n";
      }

      // now, time to traverse right, push a 1 to indicate a right traversal
      code.push_back(1);
      inOrderCoding(root->right, code); // call on right node

      // done with tree section, remove previous value from coding
      if (!code.empty()) { // check that vector is not empty
         code.pop_back();
      }
   } else {
      // a null pointer has been reached, remove previous 0/1 push from the
      // function call above
      if (!code.empty()) { // check that vector is not empty
         code.pop_back();
      }
   }

   return;
}

int main() {
   cout << "Huffman Coding:" << endl;

   // vector to store letter object nodes
   vector<Node*> letters;

   // populate vector full of classes representing letters
   populateVector(letters);
   import(letters); // import data from txt file into letter objects
   //removeZeroEntries(letters); // remove any letters with zero instances

   // create root pointing at the huffman tree
   Node* root = buildHuffmanTree(letters);
   inOrder(root); // print in order

   // create a vector to store the code for each letter
   vector<int> code;
   inOrderCoding(root, code); // print the huffman codes for each letter

   cout << "Program End." << endl;

   return 0;
}