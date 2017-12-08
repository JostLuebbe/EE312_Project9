/*
 * Author: Jost Luebbe
 */

#include "Parse.h"
#include "BST.h"
#include "Project9.h"

BST sym_tab;

int evaluate_exp(){
    vector<Node*> expr;
    while((curToken->tokenType != KEYWORD) && (curToken->tokenType != END) && (curToken->tokenString != "//")){
        switch(curToken->tokenType){
            case NUMBER:
                expr.push_back(new Node(true, curToken->numValue, "\0"));
                break;
            case SYMBOL:{
                String symbol = curToken->tokenString;
                expr.push_back(new Node(false, 0, symbol));
                if(symbol == "!" or symbol == "~")
                    expr.push_back(new Node(true, 999, "\0"));
                break;
            }
            case STRING:{
                String var_name = curToken->tokenString;
                int var_value = sym_tab.get_value(var_name);
                expr.push_back(new Node(true, var_value, "\0"));
                break;
            }
        }
        if(curToken->tokenType == KEYWORD){
            break;
        }
        read_next_token();
    }
    ExpTree exprTree(expr);

    int ret = exprTree.parse();
    return ret;
}

int evaluate_exp(vector<Token*> input){
    vector<Node*> expr;
    int i = 0;
    while(input[i]->tokenType != END){
        switch(input[i]->tokenType){
            case NUMBER:
                expr.push_back(new Node(true, input[i]->numValue, "\0"));
                break;
            case SYMBOL:{
                String symbol = input[i]->tokenString;
                expr.push_back(new Node(false, 0, symbol));
                if(symbol == "!" or symbol == "~")
                    expr.push_back(new Node(true, 999, "\0"));
                break;
            }
            case STRING:{
                String var_name = input[i]->tokenString;
                int var_value = sym_tab.get_value(var_name);
                expr.push_back(new Node(true, var_value, "\0"));
                break;
            }
        }
        i++;
    }

    ExpTree exprTree(expr);
    int ret = exprTree.parse();
    return ret;
}

void text_command(){
    read_next_token();
    String text_output = curToken->tokenString;
    cout << text_output.c_str();
}

void text_command(vector<Token*> input){
    String text_output = input[1]->tokenString;
    cout << text_output.c_str();
}

void output_command(){
    read_next_token();
    switch(curToken->tokenType){
        case NUMBER:{
            int output = curToken->numValue;
            cout << output;
            break;
        }
        case SYMBOL:{
            int exp_val = evaluate_exp();
            cout << exp_val;
            break;
        }
        case STRING:{
            String var_name = curToken->tokenString;
            int var_value = sym_tab.get_value(var_name);
            cout << var_value;
            break;
        }
    }
}

void output_command(vector<Token*> input){
    switch(input[1]->tokenType){
        case NUMBER:{
            int output = input[1]->numValue;
            cout << output;
            break;
        }
        case SYMBOL:{
            input.erase(input.begin());
            int exp_val = evaluate_exp(input);
            cout << exp_val;
            break;
        }
        case STRING:{
            String var_name = input[1]->tokenString;
            int var_value = sym_tab.get_value(var_name);
            cout << var_value;
            break;
        }
    }
}

void var_command(){
    read_next_token();
    String name = curToken->tokenString;
    if(sym_tab.is_present(name))
        cout << "variable " << name.c_str() << " incorrectly re-initialized" << endl;
    read_next_token();
    switch(curToken->tokenType){
        case NUMBER:{
            sym_tab.insert(name, curToken->numValue);
            break;
        } case STRING:{
            String var_name = curToken->tokenString;
            int var_value = sym_tab.get_value(var_name);
            sym_tab.insert(name, var_value);
            break;
        } case SYMBOL:{
            int exp_value = evaluate_exp();
            sym_tab.insert(name, exp_value);
            break;
        }
    }
}

void var_command(vector<Token*> input){
    String name = input[1]->tokenString;
    if(sym_tab.is_present(name))
        cout << "variable " << name.c_str() << " incorrectly re-initialized" << endl;
    switch(input[2]->tokenType){
        case NUMBER:{
            sym_tab.insert(name, input[2]->numValue);
            break;
        } case STRING:{
            String var_name = input[2]->tokenString;
            int var_value = sym_tab.get_value(var_name);
            sym_tab.insert(name, var_value);
            break;
        } case SYMBOL:{
            input.erase(input.begin());
            int exp_value = evaluate_exp(input);
            sym_tab.insert(name, exp_value);
            break;
        }
    }
}

void set_command(){
    read_next_token();
    String name = curToken->tokenString;
    if(!(sym_tab.is_present(name)))
        cout << "variable " << name.c_str() << " not declared" << endl;
    read_next_token();
    switch(curToken->tokenType){
        case NUMBER:{
            sym_tab.insert(name, curToken->numValue);
            break;
        } case STRING:{
            String var_name = curToken->tokenString;
            int var_value = sym_tab.get_value(var_name);
            sym_tab.insert(name, var_value);
            break;
        } case SYMBOL:{
            int exp_value = evaluate_exp();
            sym_tab.insert(name, exp_value);
            break;
        }
    }
}

void set_command(vector<Token*> input){
    String name = input[1]->tokenString;
    if(!(sym_tab.is_present(name)))
        cout << "variable " << name.c_str() << " not declared" << endl;
    switch(input[2]->tokenType){
        case NUMBER:{
            sym_tab.insert(name, input[2]->numValue);
            break;
        } case STRING:{
            String var_name = input[2]->tokenString;
            int var_value = sym_tab.get_value(var_name);
            sym_tab.insert(name, var_value);
            break;
        } case SYMBOL:{
            input.erase(input.begin());
            input.erase(input.begin());
            int exp_value = evaluate_exp(input);
            sym_tab.insert(name, exp_value);
            break;
        }
    }
}

void do_command(){
    read_next_token();

    vector<Token*> cond;
    while(curToken->tokenType != KEYWORD){
        cond.push_back(new Token(curToken->tokenType, curToken->numValue, curToken->tokenString));
        read_next_token();
        if(curToken->tokenString == "//"){
            skip_line();
            read_next_token();
        }
    }
    cond.push_back(new Token(END, 0, "END"));

    vector<vector<Token*>> inner_loop;

    while(curToken->tokenString != "od"){
        int identifier = 0;
        if(curToken->tokenString == "do"){
            identifier = 1;
        }else if(curToken->tokenString=="if"){
            identifier = 2;
        }

        if(curToken->tokenString == "//"){
            skip_line();
            read_next_token();
        }

        vector<Token*> stat;

        switch(identifier){
            case 0:
                stat.push_back(new Token(curToken->tokenType, curToken->numValue, curToken->tokenString));
                read_next_token();
                while(curToken->tokenType != KEYWORD){
                    if(curToken->tokenString == "//"){
                        skip_line();
                        read_next_token();
                        if(curToken->tokenString == "od")
                            continue;
                    }
                    stat.push_back(new Token(curToken->tokenType, curToken->numValue, curToken->tokenString));
                    read_next_token();
                }
                stat.push_back(new Token(END, 0, "END"));
                break;
            case 1:{
                int do_cnt = -1;
                while(curToken->tokenString != "od" || do_cnt){
                    if(curToken->tokenString == "//"){
                        skip_line();
                        read_next_token();
                        if(curToken->tokenString == "od")
                            continue;
                    }
                    if(curToken->tokenString == "do")
                        do_cnt++;
                    else if(curToken->tokenString=="od")
                        do_cnt--;
                    stat.push_back(new Token(curToken->tokenType, curToken->numValue, curToken->tokenString));
                    read_next_token();
                }
                stat.push_back(new Token(KEYWORD, 0, "od"));
                read_next_token();
                break;
            }
            case 2:{
                int if_cnt = -1;
                while(curToken->tokenString != "fi" || if_cnt){
                    if(curToken->tokenString == "//"){
                        skip_line();
                        read_next_token();
                        if(curToken->tokenString == "od")
                            continue;
                    }
                    if(curToken->tokenString == "if")
                        if_cnt++;
                    else if(curToken->tokenString=="fi")
                        if_cnt--;
                    stat.push_back(new Token(curToken->tokenType, curToken->numValue, curToken->tokenString));
                    read_next_token();
                }
                stat.push_back(new Token(KEYWORD, 0, "fi"));
                read_next_token();
                break;
            }
        }
        inner_loop.push_back(stat);
    }

    while(evaluate_exp(cond)) {
        for (auto &i : inner_loop) {
            if (i[0]->tokenString == "text") {
                text_command(i);
            } else if (i[0]->tokenString == "output") {
                output_command(i);
            } else if (i[0]->tokenString == "var") {
                var_command(i);
            } else if (i[0]->tokenString == "set") {
                set_command(i);
            } else if (i[0]->tokenString == "do") {
                do_command(i);
            } else if (i[0]->tokenString == "if") {
                if_command(i);
            }
        }
    }
    if(curToken->tokenString == "//")
        skip_line();
    read_next_token();
}

void do_command(vector<Token*> input) {
    input.erase(input.begin());

    vector<Token*> cond;
    while(input[0]->tokenType != KEYWORD){
        cond.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
        input.erase(input.begin());
        if(input[0]->tokenString == "//"){
            input.erase(input.begin());
            input.erase(input.begin());
        }
    }
    cond.push_back(new Token(END, 0, "END"));

    vector<vector<Token*>> inner_loop;
    while(input[0]->tokenString != "od"){
        int identifier = 0;
        if(input[0]->tokenString == "do"){
            identifier = 1;
        }else if(input[0]->tokenString=="if"){
            identifier = 2;
        }
        vector<Token*> stat;

        if(input[0]->tokenString == "//"){
            input.erase(input.begin());
            input.erase(input.begin());
        }

        switch(identifier){
            case 0:
                stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                input.erase(input.begin());
                while(input[0]->tokenType != KEYWORD){
                    if(input[0]->tokenString == "//"){
                        input.erase(input.begin());
                        input.erase(input.begin());
                    }
                    stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                    input.erase(input.begin());
                }
                stat.push_back(new Token(END, 0, "END"));
                break;
            case 1:{
                int do_cnt = -1;
                while(input[0]->tokenString != "od" || do_cnt){
                    if(input[0]->tokenString == "//"){
                        input.erase(input.begin());
                        input.erase(input.begin());
                    }
                    if(input[0]->tokenString == "do")
                        do_cnt++;
                    else if(input[0]->tokenString=="od")
                        do_cnt--;
                    stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                    input.erase(input.begin());
                }
                stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                input.erase(input.begin());
                break;
            }
            case 2:{
                int if_cnt = -1;
                while(input[0]->tokenString != "fi" || if_cnt){
                    if(input[0]->tokenString == "//"){
                        input.erase(input.begin());
                        input.erase(input.begin());
                    }
                    if(input[0]->tokenString == "if")
                        if_cnt++;
                    else if(input[0]->tokenString=="fi")
                        if_cnt--;
                    stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                    input.erase(input.begin());
                }
                stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                input.erase(input.begin());
                break;
            }
        }
        inner_loop.push_back(stat);
    }

    while(evaluate_exp(cond)) {
        for (auto &i : inner_loop) {
            if (i[0]->tokenString == "text") {
                text_command(i);
            } else if (i[0]->tokenString == "output") {
                output_command(i);
            } else if (i[0]->tokenString == "var") {
                var_command(i);
            } else if (i[0]->tokenString == "set") {
                set_command(i);
            } else if (i[0]->tokenString == "do") {
                do_command(i);
            } else if (i[0]->tokenString == "if") {
                if_command(i);
            }
        }
    }
}

void if_command(){
    read_next_token();

    vector<Token*> cond;
    while(curToken->tokenType != KEYWORD){
        cond.push_back(new Token(curToken->tokenType, curToken->numValue, curToken->tokenString));
        read_next_token();
        if(curToken->tokenString == "//"){
            skip_line();
            read_next_token();
        }
    }
    cond.push_back(new Token(END, 0, "END"));

    vector<vector<Token*>> if_loop;
    while(curToken->tokenString != "fi" and curToken->tokenString != "else"){
        int identifier = 0;
        if(curToken->tokenString == "do"){
            identifier = 1;
        }else if(curToken->tokenString=="if"){
            identifier = 2;
        }
        vector<Token*> stat;

        if(curToken->tokenString == "//"){
            skip_line();
            read_next_token();
        }

        switch(identifier){
            case 0:
                stat.push_back(new Token(curToken->tokenType, curToken->numValue, curToken->tokenString));
                read_next_token();
                while(curToken->tokenType != KEYWORD){
                    if(curToken->tokenString == "//"){
                        skip_line();
                        read_next_token();
                    }
                    stat.push_back(new Token(curToken->tokenType, curToken->numValue, curToken->tokenString));
                    read_next_token();
                }
                stat.push_back(new Token(END, 0, "END"));
                break;
            case 1:{
                int do_cnt = -1;
                while(curToken->tokenString != "od" || do_cnt){
                    if(curToken->tokenString == "//"){
                        skip_line();
                        read_next_token();
                    }
                    if(curToken->tokenString == "do")
                        do_cnt++;
                    else if(curToken->tokenString=="od")
                        do_cnt--;
                    stat.push_back(new Token(curToken->tokenType, curToken->numValue, curToken->tokenString));
                    read_next_token();
                }
                stat.push_back(new Token(KEYWORD, 0, "od"));
                read_next_token();
                break;
            }
            case 2:{
                int if_cnt = -1;
                while(curToken->tokenString != "fi" || if_cnt){
                    if(curToken->tokenString == "//"){
                        skip_line();
                        read_next_token();
                    }
                    if(curToken->tokenString == "if")
                        if_cnt++;
                    else if(curToken->tokenString=="fi")
                        if_cnt--;
                    stat.push_back(new Token(curToken->tokenType, curToken->numValue, curToken->tokenString));
                    read_next_token();
                }
                stat.push_back(new Token(KEYWORD, 0, "fi"));
                read_next_token();
                break;
            }
        }
        if_loop.push_back(stat);
    }

    vector<vector<Token*>> else_loop;
    if(curToken->tokenString == "else"){
        read_next_token();
        while(curToken->tokenString != "fi"){
            int identifier = 0;
            if(curToken->tokenString == "do"){
                identifier = 1;
            }else if(curToken->tokenString=="if"){
                identifier = 2;
            }
            vector<Token*> stat;

            if(curToken->tokenString == "//"){
                skip_line();
                read_next_token();
            }

            switch(identifier){
                case 0:
                    stat.push_back(new Token(curToken->tokenType, curToken->numValue, curToken->tokenString));
                    read_next_token();
                    while(curToken->tokenType != KEYWORD){
                        if(curToken->tokenString == "//"){
                            skip_line();
                            read_next_token();
                        }
                        stat.push_back(new Token(curToken->tokenType, curToken->numValue, curToken->tokenString));
                        read_next_token();
                    }
                    stat.push_back(new Token(END, 0, "END"));
                    break;
                case 1:{
                    int do_cnt = -1;
                    while(curToken->tokenString != "od" || do_cnt){
                        if(curToken->tokenString == "//"){
                            skip_line();
                            read_next_token();
                        }
                        if(curToken->tokenString == "do")
                            do_cnt++;
                        else if(curToken->tokenString=="od")
                            do_cnt--;
                        stat.push_back(new Token(curToken->tokenType, curToken->numValue, curToken->tokenString));
                        read_next_token();
                    }
                    stat.push_back(new Token(KEYWORD, 0, "od"));
                    read_next_token();
                    break;
                }
                case 2:{
                    int if_cnt = -1;
                    while(curToken->tokenString != "fi" || if_cnt){
                        if(curToken->tokenString == "//"){
                            skip_line();
                            read_next_token();
                        }
                        if(curToken->tokenString == "if")
                            if_cnt++;
                        else if(curToken->tokenString=="fi")
                            if_cnt--;
                        stat.push_back(new Token(curToken->tokenType, curToken->numValue, curToken->tokenString));
                        read_next_token();
                    }
                    stat.push_back(new Token(KEYWORD, 0, "fi"));
                    read_next_token();
                    break;
                }
            }
            else_loop.push_back(stat);
        }
    }

    if(evaluate_exp(cond)){
        for (auto &i : if_loop) {
            if (i[0]->tokenString == "text") {
                text_command(i);
            } else if (i[0]->tokenString == "output") {
                output_command(i);
            } else if (i[0]->tokenString == "var") {
                var_command(i);
            } else if (i[0]->tokenString == "set") {
                set_command(i);
            } else if (i[0]->tokenString == "do") {
                do_command(i);
            } else if (i[0]->tokenString == "if") {
                if_command(i);
            }
        }
    }
    else{
        for (auto &i : else_loop) {
            if (i[0]->tokenString == "text") {
                text_command(i);
            } else if (i[0]->tokenString == "output") {
                output_command(i);
            } else if (i[0]->tokenString == "var") {
                var_command(i);
            } else if (i[0]->tokenString == "set") {
                set_command(i);
            } else if (i[0]->tokenString == "do") {
                do_command(i);
            } else if (i[0]->tokenString == "if") {
                if_command(i);
            }
        }
    }
    read_next_token();
}

void if_command(vector<Token*> input){
    input.erase(input.begin());

    vector<Token*> cond;
    while(input[0]->tokenType != KEYWORD){
        cond.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
        input.erase(input.begin());
        if(input[0]->tokenString == "//"){
            input.erase(input.begin());
            input.erase(input.begin());
        }
    }
    cond.push_back(new Token(END, 0, "END"));

    vector<vector<Token*>> if_loop;
    while(input[0]->tokenString != "fi" and input[0]->tokenString != "else"){
        int identifier = 0;
        if(input[0]->tokenString == "do"){
            identifier = 1;
        }else if(input[0]->tokenString=="if"){
            identifier = 2;
        }
        vector<Token*> stat;

        if(input[0]->tokenString == "//"){
            input.erase(input.begin());
            input.erase(input.begin());
        }

        switch(identifier){
            case 0:
                stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                input.erase(input.begin());
                while(input[0]->tokenType != KEYWORD){
                    if(input[0]->tokenString == "//"){
                        input.erase(input.begin());
                        input.erase(input.begin());
                    }
                    stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                    input.erase(input.begin());
                }
                stat.push_back(new Token(END, 0, "END"));
                break;
            case 1:{
                int do_cnt = -1;
                while(input[0]->tokenString != "od" || do_cnt){
                    if(input[0]->tokenString == "//"){
                        input.erase(input.begin());
                        input.erase(input.begin());
                    }
                    if(input[0]->tokenString == "do")
                        do_cnt++;
                    else if(input[0]->tokenString=="od")
                        do_cnt--;
                    stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                    input.erase(input.begin());
                }
                stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                input.erase(input.begin());
                break;
            }
            case 2:
                int if_cnt = -1;
                while(input[0]->tokenString != "fi" || if_cnt){
                    if(input[0]->tokenString == "//"){
                        input.erase(input.begin());
                        input.erase(input.begin());
                    }
                    if(input[0]->tokenString == "if")
                        if_cnt++;
                    else if(input[0]->tokenString=="fi")
                        if_cnt--;
                    stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                    input.erase(input.begin());
                }
                stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                input.erase(input.begin());
                break;
        }
        if_loop.push_back(stat);
    }

    vector<vector<Token*>> else_loop;
    if(input[0]->tokenString == "else"){
        while(input[0]->tokenString != "fi"){
            int identifier = 0;
            if(input[0]->tokenString == "do"){
                identifier = 1;
            }else if(input[0]->tokenString=="if"){
                identifier = 2;
            }
            vector<Token*> stat;

            if(input[0]->tokenString == "//"){
                input.erase(input.begin());
                input.erase(input.begin());
            }

            switch(identifier){
                case 0:
                    stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                    input.erase(input.begin());
                    while(input[0]->tokenType != KEYWORD){
                        if(input[0]->tokenString == "//"){
                            input.erase(input.begin());
                            input.erase(input.begin());
                        }
                        stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                        input.erase(input.begin());
                    }
                    stat.push_back(new Token(END, 0, "END"));
                    break;
                case 1:{
                    int do_cnt = -1;
                    while(input[0]->tokenString != "od" || do_cnt){
                        if(input[0]->tokenString == "//"){
                            input.erase(input.begin());
                            input.erase(input.begin());
                        }
                        if(input[0]->tokenString == "do")
                            do_cnt++;
                        else if(input[0]->tokenString=="od")
                            do_cnt--;
                        stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                        input.erase(input.begin());
                    }
                    stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                    input.erase(input.begin());
                    break;
                }
                case 2:{
                    int if_cnt = -1;
                    while(input[0]->tokenString != "fi" || if_cnt){
                        if(input[0]->tokenString == "//"){
                            input.erase(input.begin());
                            input.erase(input.begin());
                        }
                        if(input[0]->tokenString == "if")
                            if_cnt++;
                        else if(input[0]->tokenString=="fi")
                            if_cnt--;
                        stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                        input.erase(input.begin());
                    }
                    stat.push_back(new Token(input[0]->tokenType, input[0]->numValue, input[0]->tokenString));
                    input.erase(input.begin());
                    break;
                }
            }
            else_loop.push_back(stat);
        }
    }

    if(evaluate_exp(cond)){
        for (auto &i : if_loop) {
            if (i[0]->tokenString == "text") {
                text_command(i);
            } else if (i[0]->tokenString == "output") {
                output_command(i);
            } else if (i[0]->tokenString == "var") {
                var_command(i);
            } else if (i[0]->tokenString == "set") {
                set_command(i);
            } else if (i[0]->tokenString == "do") {
                do_command(i);
            } else if (i[0]->tokenString == "if") {
                if_command(i);
            }
        }
    }
    else{
        for (auto &i : else_loop) {
            if (i[0]->tokenString == "text") {
                text_command(i);
            } else if (i[0]->tokenString == "output") {
                output_command(i);
            } else if (i[0]->tokenString == "var") {
                var_command(i);
            } else if (i[0]->tokenString == "set") {
                set_command(i);
            } else if (i[0]->tokenString == "do") {
                do_command(i);
            } else if (i[0]->tokenString == "if") {
                if_command(i);
            }
        }
    }
}

void run(){
    read_next_token();
    while(curToken->tokenType != END){
        switch(curToken->tokenType){
            case KEYWORD:
                if(curToken->tokenString == "text"){
                    text_command();
                } else if(curToken->tokenString == "output"){
                    output_command();
                } else if(curToken->tokenString == "var"){
                    var_command();
                } else if(curToken->tokenString == "set"){
                    set_command();
                } else if(curToken->tokenString == "do"){
                    do_command();
                } else if(curToken->tokenString == "if"){
                    if_command();
                }
            case SYMBOL:
                if(curToken->tokenString == "//")
                    skip_line();
                break;
            default:
                printf("WHATS GOING ON HERE");
                break;
        }
        if(curToken->tokenType == KEYWORD){
            continue;
        }
        if(curToken->tokenType == END){
            break;
        }
        read_next_token();
    }
}