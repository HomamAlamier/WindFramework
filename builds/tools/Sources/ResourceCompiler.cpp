#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <memory.h>
#include <sstream>
#ifndef _WIN32
constexpr char sp_char = '/';
#else
constexpr char sp_char = '\\';
#endif // !_WIN32
using String = std::string;

void zero_buffer(char* buffer, size_t count){
    for (size_t i = 0; i < count; i++)
    {
        buffer[i] = 0;
    }
}
String refactor_string_array(String code){
    String str = "\"";
    for(size_t i = 0; i < code.size(); i++) {
        String cd;
        cd += (char)code[i];
        cd += (char)code[i + 1];
        if (cd == "\\n") { 
            if (i != code.size() - 2) str += "\\n\"\n\"";     
            i++; 
        } 
        else { str += (char)code[i]; }
        if (i == code.size() - 1) {
            std::cout << "lastchar[i] = " << code[i] << std::endl;
            str += "\"";
        }
    }
    return str;
}
String convert_string_to_code(String str){
    String out = "";

    char src_chars[5] =  { '\n', '\"', '\'', '\\', '\r' };
    String rep_strs[5] = { "\\n", "\\\"" , "\\\'", "\\\\", "\\r" };



    for(auto _ch : str){
        bool fnd = false;
        for (size_t i = 0; i < 5; i++)
        {

            if (src_chars[i] == _ch){
                out += rep_strs[i];
                fnd = true;
                break;
            }
        }  
        if (!fnd) out += _ch;  
    }
    return out;
}
String get_file_name(String str){
    size_t ind = str.find_last_of(sp_char),
        ind2 = str.find_last_of('.');
    return str.substr(ind + 1, ind2 - (ind + 1));
}
int main(int argc, const char** argv){
    int c_type = 0;
    bool in_file = false, verbose = false, append = false;
    std::cout << "Args List:" << std::endl;
    for (size_t i = 0; i < argc; i++)
    {
        std::cout << "\t" << std::to_string(i) << ": " << argv[i] << std::endl;
    }
    std::vector<String> in_files;
    String out_file;
    for (size_t i = 1; i < argc; i++)
    {
        String arg(argv[i]);
        if (arg == "-v") verbose = true;
        else if (arg == "-string") c_type = 1;
        else if (arg == "-binary") c_type = 2;
        else if (arg == "-app") append = true;
        else if (arg == "-o"){
            out_file = String(argv[i + 1]);
            i++;
        }
        else {
            in_files.push_back(arg);
        }
    }
    if (c_type == 0){
        perror("Please Select a convert type by using -string or -binary !");
        return EXIT_FAILURE;
    }
    if (out_file == ""){
        perror("Please select an output file ! (-o [PATH]).");
        return EXIT_FAILURE;
    }
    if (in_files.size() == 0){
        perror("Please select one or more input files !");
        return EXIT_FAILURE;
    }
    String code = "";

    std::cout << "Output: " << std::endl;

    std::fstream outStream(out_file, (append ? std::ios::app : std::ios::out));
    for (auto f : in_files){
        std::cout << "\tOpening " << f << "...";
        std::fstream inStream(f, std::ios::in);
        if (inStream.good()) { std::cout << "OK !" << std::endl; }
        else {
            std::cout << "ERROR !" << std::endl;
            continue;
        }
        if (c_type == 1){
            code += "const char " + get_file_name(f) + "[] = \n";
            std::cout << "\tGenerating " << "const char " + get_file_name(f) << "[] ...";
            char buffer[15001];
            buffer[15000] = 0;
            while (!inStream.eof()){
                zero_buffer(buffer, 15000);
                buffer[inStream.read(buffer, 15000).gcount() + 1] = 0;
                String line(buffer);
                code += refactor_string_array(convert_string_to_code(line));
            }
            code += ";\n";
            std::cout << "Done !" << std::endl;
        }
        else if (c_type == 2){
            struct stat g;
            if (stat(f.c_str(), &g) == 0){
                size_t cur = 0;
                size_t ocur = 0;
                code += "static unsigned char " + get_file_name(f) + "_bytes[] = \n{\n";
                std::cout << "\tGenerating unsigned char array with width of " << std::to_string(g.st_size) << "...";
                std::stringstream sstr;
                while (!inStream.eof()){
                    size_t ss = cur + 1024 > g.st_size ? g.st_size - cur : 1024;
                    char* buffer = new char[ss];
                    auto cc = inStream.read(buffer, ss).gcount();
                    if (cc == 0) break;
                    for (size_t i = 0; i < cc; i++)
                    {
                        ocur++;
                        if (ocur == 20){
                            sstr << "\n";
                            ocur = 0;
                        }
                        if (cur + i == g.st_size - 1)
                            sstr << "0x" << std::hex << (int)static_cast<unsigned char>(buffer[i]);
                        else
                            sstr << "0x" << std::hex << (int)static_cast<unsigned char>(buffer[i]) << ", ";
                    }                       
                    cur += cc;          
                }
                code += sstr.str() + "\n};";
                std::cout << "Done !" << std::endl;
            } 
            else {
                std::cout << "\tError Getting file status !" << std::endl;
                continue;
            }
        }
        inStream.close();
    }
    outStream.write(code.c_str(), code.size());
    outStream.close();
}
