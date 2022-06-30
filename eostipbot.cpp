#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>

using eosio::name;
using eosio::datastream;
namespace eosio{
class [[eosio::contract("eostipbot")]] eostipbot : public contract { 
    
 public:
    eostipbot(name receiver, name code, datastream<const char*> ds)
        :eosio::contract(receiver, code, ds),
        platforms(_self, _self.value),
        tokens(_self, _self.value),
        tokendefs(_self, _self.value),
        platformdefs(_self, _self.value),
        users(_self, _self.value),
        balances(_self, _self.value),
        testers(_self, _self.value){}
        
   

   [[eosio::action]]
    void sendtip(name name, std::string touser,std::string tomemo, std::string fromuser,std::string frommemo, std::string platform, std::string platformid, std::string whotype, std::string symbol, std::string amount, std::string memo, std::string prevtx, std::string previd, std::string contract){
	    require_auth(_self);
    }

    [[eosio::action]]
    void sendtipram(name name, std::string touser,std::string tomemo, 
    std::string fromuser,std::string frommemo, std::string platform, 
    std::string platformid, std::string whotype, std::string symbol, 
    double amount, std::string memo, std::string prevtx, std::string previd, 
    std::string contract){
	    //auto contractnametoplevel = "learnwithjoy"; 
        require_auth(_self);
        auto pid = std::stoi(platformid);
        auto amt = amount;
        auto platid = pid;
        auto tokenid = get_token_id(symbol, contract);
        auto toid = get_user_info_by_uid(tomemo);
        auto fromid = get_user_info_by_uid(frommemo);

        auto plat = platforms.find(platid);
        auto token = tokens.find(tokenid);
        
        returnuser * to = new returnuser{};
        if(get_user_info_by_uid(tomemo) != 0){
            //addTest(tomemo, "1", "User Match", "to", "");
            to->pid = pid;
            to->utype = 1;
            to->userid = tomemo;
            to->username = touser;
        }else if (get_platform_by_userid(tomemo) != 0){
            //addTest(tomemo, "2", "Platform Match", "to", "");
            to->pid = pid;
            to->utype = 2;
            to->userid = tomemo;
            to->username = touser;
        }else{
            //addTest(tomemo, "3", "No Match", "to", "");
            to->pid = pid;
            to->utype = 3;
            to->userid = tomemo;
            to ->username = touser;
        }

        returnuser * from = new returnuser{};
        if(get_user_info_by_uid(frommemo) != 0){
            //addTest(tomemo, "1", "User Match", "from", "");
            from->pid = pid;
            from->utype = 1;
            from->userid = tomemo;
            from->username = touser;
        }else if (get_platform_by_userid(frommemo) != 0){
            //addTest(tomemo, "2", "Platform Match", "from", "");
            from->pid = pid;
            from->utype = 2;
            from->userid = tomemo;
            from->username = touser;
        }else{
            //addTest(tomemo, "31", "No Match", "from", "");
            from->pid = pid;
            from->utype = 3;
            from->userid = tomemo;
            from ->username = touser;
        }

        //returnuser * to = get_user_info(pid, tomemo, touser);
        //returnuser * from = get_user_info(pid, frommemo, fromuser);
        auto tu = to->utype;
        auto touid = to->userid;
        auto toname = to->username;
        auto fu = from->utype;
        auto fid = from->userid;
        auto fname = from->username;
        //addTest(std::to_string(tu), touid, toname, "To", "");
        //addTest(std::to_string(fu), fid, fname, "From", "");
        if(tu == 3 && fu != 3){
            //addTest(tomemo, "Was a 3 on tu but not fu", "No Match", "001", "a");
            //create user
            if(tu == 3){
                auto npid = platforms.available_primary_key();
                addplatobj(name, pid, 0,  tomemo, toname, npid);
                addbalobj(name, token->id, tomemo, amt);
                modifybal(pid,frommemo, token->id, amt, false);
            }
        }else{
            //addTest(tomemo, "On the else", "Matching", "001", "b");
            if(tu == 1 || tu == 2){
                //addTest(tomemo, "About to call modify to", "Matching", "001", "c");
                modifybal(pid,tomemo, token->id, amt, true);
            }
            if(fu == 1 || fu == 2){
                //addTest(tomemo, "About to call modify to", "Matching", "001", "d");
                modifybal(pid,frommemo, token->id, amt, false);
            }            
        }
    }

    void addTest(std::string uid, std::string  a, std::string  b, std::string c, std::string d){
         require_auth(_self);
         testers.emplace(_self, [&](auto& tst) {
            tst.id = testers.available_primary_key();
            tst.userid = uid;
            tst.a = a;
            tst.b = b;
            tst.c = c;
            tst.d = d;
        });
    }

    [[eosio::action]]
    void withdrawal(name name, std::string eosto,std::string eosfrom, std::string from,std::string fromemo, std::string platform,std::string symbol, std::string amount,std::string memo, std::string prevtxid,std::string contract, std::string witxid){
	  require_auth(_self);

    }
    
    [[eosio::action]]
    void deposit(name name, std::string eosto, std::string eosfrom,std::string from, std::string fromemo, std::string platform, std::string symbol,std::string amount,std::string memo, std::string deptxid,std::string prevtxid, std::string contract){
		require_auth(_self);
    

    }

     [[eosio::action]]
    void depositram(name name, std::string eosto, std::string eosfrom,std::string from, std::string fromemo, std::string platform, std::string symbol,double amount,std::string memo, std::string deptxid,std::string prevtxid, std::string contract){
		require_auth(_self);
        auto npid = platforms.available_primary_key();
        //addplatobj(name, npid, 0,  tomemo, toname, npid);
        //addbalobj(name, token.id, tomemo, amount);
        //modifybal(npid,fromemo, token.id, amount, true);
    }
	
    [[eosio::action]]
    void checkuser(name user) {
		require_auth(_self);
    }

    [[eosio::action]]
    void newuser(name uid, std::string username, std::string nickname, std::string memo){
        require_auth(_self);
        users.emplace(_self, [&](auto& new_user) {
            new_user.id = users.available_primary_key();
            new_user.username = username;
            new_user.uid = uid;
            new_user.userid = uid.value;
            new_user.nickname = nickname;
      });
    }
  
    [[eosio::action]]  
    void modplatform(name uid,std::string username, uint64_t pid,std::string userid){
        require_auth(_self);
        platforms.emplace(_self, [&](auto& plat) {
            plat.id = platforms.available_primary_key();
            plat.pid = pid;
            plat.userid =  userid;
            plat.uid = uid.value;
            plat.username = username;
      });
    }

    [[eosio::action]]
    void addpd(name username, std::string platformname, std::string authtype){
        require_auth(_self);
        platformdefs.emplace(_self, [&](auto& pfd) {
            pfd.id  = platformdefs.available_primary_key();
            pfd.platformname = platformname;
            pfd.authtype = authtype;
      });
    }

    [[eosio::action]]
    void addtoken(name username, std::string symbol, std::string contract, std::string chainid, std::string tokentype, uint64_t precision){
        require_auth(_self);
        tokendefs.emplace(_self, [&](auto& tkn) {
            tkn.id = tokendefs.available_primary_key();
            tkn.symbol = symbol;
            tkn.contract = contract;
            tkn.chainid = chainid;
            tkn.tokentype = tokentype;
            tkn.precision = precision;
        });
    } 

    [[eosio::action]]
    void addbalobj(name username, uint64_t tid, std::string userid, double amount){
        require_auth(_self);
        balances.emplace(_self, [&](auto& bln) {
            bln.id = balances.available_primary_key();
            bln.tid = tid;
            bln.userid = userid;
            bln.amount = amount;
            bln.version = 3;
        });
    } 

    [[eosio::action]]
    void delbalobj(uint64_t id){
        require_auth(_self);
        uint64_t count = 0;
        // iterate over first index 
        auto itr = balances.find(id);
        balances.erase(itr);
    }


    [[eosio::action]]
    void addplatobj(name name, uint64_t pid, uint64_t uid, std::string userid, std::string username, uint64_t id){
        auto ppid = 999999;
        if(id == 999999){
            ppid = platforms.available_primary_key();
        }else{
            ppid = id;
        }
        require_auth(_self);
        platforms.emplace(_self, [&](auto& plm) {
            plm.id = ppid;
            plm.pid = pid;
            plm.uid = uid;
            plm.userid = userid;
            plm.username  = username;
        });
    } 
    
    [[eosio::action]]
    void delplatobj(uint64_t id){
        require_auth(_self);
        uint64_t count = 0;
        // iterate over first index 
        auto it = platforms.find(id);
        platforms.erase(it);
    }

    [[eosio::action]]
    void delplatdefobj(uint64_t id){
        require_auth(_self);
        uint64_t count = 0;
        // iterate over first index 
        auto it = platformdefs.find(id);
        platformdefs.erase(it);
    }

    [[eosio::action]]
    void modbalance(name authname, std::string username, uint64_t pid, std::string userid, uint64_t tid, double amount){
        require_auth(_self);
        bool nouser = false;
        bool noplat = false;
        auto plats = platforms.find(pid);
        uint64_t toker = tokens.find(tid)->id;
        std::vector<uint64_t> ballist = {};
        auto recuid = 0;
        if(plats==platforms.end())
        {
            noplat = true;
            auto uidd = platforms.available_primary_key();
            //addbalobj(_self, tok->id, userid, amount);
            platforms.emplace(_self, [&](auto& pl){
                pl.id = uidd;
                pl.userid = userid;
                pl.username = username;
                pl.version = 3;
                pl.uid = uidd;
                pl.pid = pid;
            });
            recuid = uidd;
        }else{
            recuid = plats->uid;
        }

        // auto usr = users.find(userid);
        // if(usr==users.end())
        // {
        //     nouser = true;
        // }
        bool balend = false;
        for(auto& blnc : balances)
        {
            if (blnc.userid == std::to_string(recuid)){
                balend = true;
                ballist.emplace(ballist.begin(), blnc.id);
            }
        }
        if(balend){
            for(int i = 0;i < ballist.size();i++){
                auto bals = balances.find(ballist[i]);
            
                
                if(bals==balances.end())
                    {
                        uint64_t tokenid = bals->tid;
                        auto tok = tokens.find(tokenid);
                        eosio_assert(tok==tokens.end(), "Token not found");  
                        //addbalobj(_self, tok->id, userid, amount);
                        balances.emplace(_self, [&](auto& bl){
                            bl.userid = userid;
                            bl.version = 3;
                            bl.id = balances.available_primary_key();
                            bl.tid = toker;
                            bl.uid = recuid;
                            bl.amount = amount;
                        });
                }else{
                    balances.modify( bals, _self, [&]( auto& row ) {
                            row.amount = row.amount + amount;
                    });
                }
            }
        }
    }
    
    [[eosio::action]]
    void deletetester(){
        require_auth(_self);
        std::vector<int> idlist;
        for(auto& tst : testers)
        {
            idlist.push_back(tst.id);
        }
        for(auto& tst : idlist)
        {
            auto iter = testers.find(tst);
            testers.erase(iter);
        }
        
    }

    [[eosio::action]]
    void delcoins(){
        require_auth(_self);
        std::vector<int> idlist;
        for(auto& tst : tokens)
        {
            idlist.push_back(tst.id);
        }
        for(auto& tst : idlist)
        {
            auto iter = tokens.find(tst);
            tokens.erase(iter);
        }
    }

    [[eosio::action]]
    void deltoken(uint64_t id){
        require_auth(_self);
        uint64_t count = 0;
        // iterate over first index 
        auto itr = tokendefs.find(id);
        tokendefs.erase(itr);
    }


    void modifybal(uint64_t pid, std::string userid, uint64_t tid, double amount, bool add){
       
    }

    uint64_t get_balance_by_ut(std::string uid,uint64_t tid){
        require_auth(_self);
        bool kilit = false;
        for(auto& bal : balances)
        {
            if ((bal.userid == uid) && bal.tid == tid){
                return bal.id;
                kilit = true;
            }
        }
        
          return 0;
    }    


    uint64_t get_user_info_by_uid(std::string uid){
        require_auth(_self);
         for(auto& user : users)
          {
              if (user.userid == uid){
                  return user.id;
              }
          }
          return 0;
    }
    
    uint64_t get_token_id(std::string symbol, std::string contract){
       require_auth(_self);
        for(auto& tkn : tokens)
          {
              if (tkn.symbol == symbol && contract == tkn.contract){
                  return tkn.id;
              }
          }
          return 0;
    }

    uint64_t get_platform_by_userid(std::string userid){
        require_auth(_self);
        for(auto& plt : platforms)
          {
              if (plt.userid == userid){
                  return plt.id;
              }
          }
          return 0;
    }

    struct returnuser {
        int64_t utype;
        std::string userid;
        std::string username;
        int64_t pid;
    };


    returnuser * get_user_info(uint64_t pid,std::string userid,std::string username){
        returnuser * ru = new returnuser{};
        if(get_user_info_by_uid(userid) != 0){
            ru->pid = pid;
            ru->utype = 1;
            ru->userid = userid;
            ru->username = username;
        }else if (get_platform_by_userid(userid) != 0){
            ru->pid = pid;
            ru->utype = 2;
            ru->userid = userid;
            ru->username = username;
        }else{
            ru->pid = pid;
            ru->utype = 3;
            ru->userid = userid;
            ru ->username = username;
        }
        return ru;
    }

 private:

 
    struct [[eosio::table]] platformdef {
        uint64_t id;
        std::string  platformname;
        std::string  authtype;
        uint64_t primary_key() const { return id;}
        std::string get_name() const {return platformname;}
        std::string get_auth_type() const {return authtype;}
        EOSLIB_SERIALIZE(platformdef, (id)(platformname)(authtype))
    };   

    struct [[eosio::table]] token {
        uint64_t id;
        std::string  symbol;
        std::string  contract;
        std::string  chainid;
        std::string  tokentype;
        uint64_t  precision;
        std::string  get_symbol() const {return symbol;}
        std::string  get_contract() const {return contract;}
        std::string  get_chainid() const {return chainid;}
        uint64_t  get_precision() const {return precision;}
        std::string get_tokentype() const {return tokentype;}
        uint64_t primary_key() const { return id; }

        EOSLIB_SERIALIZE(token, (id)(symbol)(contract)(chainid)(tokentype)(precision))
    };

    struct [[eosio::table]] tokendef {
        uint64_t id;
        std::string  symbol;
        std::string  contract;
        std::string  chainid;
        std::string  tokentype;
        uint64_t  precision;
        std::string  get_symbol() const {return symbol;}
        std::string  get_contract() const {return contract;}
        std::string  get_chainid() const {return chainid;}
        std::string get_tokentype() const {return tokentype;}
        uint64_t primary_key() const { return id; }

        EOSLIB_SERIALIZE(tokendef, (id)(symbol)(contract)(chainid)(tokentype)(precision))
    };

    struct [[eosio::table]] platform {
        uint64_t id;
        uint64_t pid;
        uint64_t uid;
        std::string userid;
        uint64_t version;
        std::string username;
        uint64_t primary_key() const { return id; }
        std::string get_username() const {return username;}
        uint64_t get_uid() const {return uid;}
        std::string get_userid() const {return userid;}
        EOSLIB_SERIALIZE(platform, (id)(pid)(uid)(userid)(version)(username))
    };

    

    struct [[eosio::table]] balance {
        uint64_t id;
        uint64_t tid;
        uint64_t uid;
        std::string userid;
        double amount;
        uint64_t version;
        std::string user_id_key() const { return userid; }
        std::string id_key() const { return userid; }
        uint64_t primary_key() const { return id;}
        double return_amount() const { return amount;};
        EOSLIB_SERIALIZE(balance, (id)(tid)(uid)(userid)(amount)(version))
    };

    struct [[eosio::table]] user {
        uint64_t id;
        name uid;
        std::string userid;
        std::string username;
        std::string  nickname;
        std::string get_nickname() const { return nickname;}
        uint64_t primary_key() const { return id; }
        std::string user_id() const {return userid;}
        name user_name() const { return uid;}

        EOSLIB_SERIALIZE(user, (id)(uid)(userid)(username)(nickname))
    }; 

     struct [[eosio::table]] tester {
        uint64_t id;
        std::string userid;
        std::string a;
        std::string b;
        std::string c;
        std::string d;
        uint64_t primary_key() const { return id; }
        std::string user_id() const {return userid;}

        EOSLIB_SERIALIZE(tester, (id)(userid)(a)(b)(c)(d))
    }; 
     

    //typedef eosio::multi_index<"balances"_n, balance, eosio::indexed_by<"pollid"_n, eosio::const_mem_fun<poll, uint64_t, &poll::by_pollId>>> pollstable;

    typedef eosio::multi_index<"tokens"_n, token> tokenIndex;
    tokenIndex tokens;
    typedef eosio::multi_index<"tokendefs"_n, tokendef> tokenDefIndex;
    tokenDefIndex tokendefs;
    typedef eosio::multi_index<"platformdefs"_n, platformdef> platDefIndex;    
    platDefIndex platformdefs;
    typedef eosio::multi_index<"platforms"_n, platform> platformIndex;
    platformIndex platforms;
    typedef eosio::multi_index<"balances"_n, balance> balanceIndex;
    balanceIndex balances;
    typedef eosio::multi_index<"users"_n, user> userIndex;
    userIndex users;
    typedef eosio::multi_index<"testers"_n, tester> testerIndex;
    testerIndex testers;

};

EOSIO_DISPATCH(eostipbot, (sendtip)(sendtipram)(withdrawal)(deposit)(depositram)(checkuser)(newuser)(addpd)(addtoken)(modbalance)(modplatform)(delplatdefobj)(addbalobj)(delbalobj)(addplatobj)(delplatobj)(deletetester)(delcoins)(deltoken))
}

