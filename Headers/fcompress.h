#ifndef FCOMPRESS_H
#define FCOMPRESS_H
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include<math.h>



class c_node{
private:
    char char_;                          // char of the cnode
    c_node*left;                         // left child if any
    c_node*right;                        // right child  if any
    std::string huff_code;               //new huffmancode to assign to the char
                           // how many time this char is occupy in whole document
public:
    int frequency;
    c_node(char char_);
    c_node(int freq);
    c_node();
    ~c_node();
    friend class Fcompress;
    friend class Compare;

};typedef c_node* nptr ;

class Compare{                       //compare fn for min heap queue
public:
    bool operator()(const c_node *x,const c_node *y)const
    {
        return (x->frequency > y->frequency);
    }
};

class Fcompress{

private:
    //private attributes
    char char_;                          //buffer to read file
    c_node* code_table[128];             //table of code indexed with ascii index (0 --> 127)
    c_node* root;
    std::priority_queue<nptr,std::vector<nptr>,Compare> min_Q;
    std::string r_filename,w_filename;
    std::fstream r_file;
    std::fstream w_file;
    // private methods
    void free_resources(c_node* p);
    void count_frq();                      // count frequencies of all charachters in document
    void create_tree();                                     // create huffman code tree using min heap (priority Q)
    void create_tree(std::string code,char char_);          // create tree from compressed file header
    void calculate_leaves(c_node* p,std::string code);
    void fill_table();                                      // fill generated huffman codes to table
    int Bin_t_Dec(std::string b);
    std::string Dec_t_Bin(int d);
    void write_to_file();
    void create_tree_from_header();
    void read_from_file();

public:
    Fcompress(std::string rfilename,std::string wfilename);
    ~Fcompress();
    void compress();
    void decompress();


};

#endif // FCOMPRESS_H
