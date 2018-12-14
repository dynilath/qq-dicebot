#include "./dice_roller.h"

#include <random>
#include <chrono>

#include "./dice_utils.h"

namespace dicebot::roll{

	#define FUNCTION_PARSE_DICE(_Str_target,_Pos_Start,_Pos_D,_Pos_K,_Pos_L) \
	int i_num_of_die = _Pos_D > 0 ? std::stoi(_Str_target.substr(_Pos_Start, _Pos_D - (_Pos_Start))) : 1;\
	int i_num_of_face = std::stoi(_Str_target.substr(_Pos_D + 1, _Pos_K - (_Pos_D + 1)));\
	int i_num_of_keep = std::stoi(_Str_target.substr(_Pos_L + 1))

	#define CHECK_DICE_LIMITS() \
	if (i_num_of_face > MAX_DICE_FACE || i_num_of_keep > MAX_DICE_FACE || i_num_of_die > MAX_DICE_NUM) { \
		this->status = roll_status::DICE_NOT_AVAILABLE; \
		break; \
	}

	#define CREATING_OUTPUT(_Stream,_DiceRoll,_Sign) \
	_Stream << "(" << *(_DiceRoll.pstr_detail_result) << ")";\
	this->i_sum_result = _DiceRoll.i_sum_result * _Sign;\
	this->pstr_detail_result = new std::string(_Stream.str())

	#define RANDOMIZE(_Max,_Min)\
	std::random_device rd_generator;\
	std::mt19937 mt_generator(ulong_prand_seed);\
	if (dice_roller::is_using_pseudo_random) mt_generator.discard(ulong_prand_stage);\
	std::uniform_int_distribution<> dice(_Min, _Max + _Min - 1)

	#define RANDOM(_Target)\
	if (dice_roller::is_using_pseudo_random) { _Target = dice(mt_generator); ulong_prand_stage++;}\
	else _Target = dice(rd_generator)

	void dice_roller::random_initialize(){
		std::random_device rd;
		if (rd.entropy() > 0.0) {
			dice_roller::is_using_pseudo_random = false;
		}
		else dice_roller::is_using_pseudo_random = true;
	}

	bool dice_roller::is_using_pseudo_random = false;
	unsigned long dice_roller::ulong_prand_seed = 0;
	unsigned long dice_roller::ulong_prand_stage = 0;

	dice_roller::dice_roller() noexcept{
	}


	dice_roller::~dice_roller(){
		if (this->pstr_detail_result != nullptr)
			delete(this->pstr_detail_result);
	}

	dice_roller::dice_roller(int val1_i_num_of_dice, int val2_num_of_face) {
		this->status = roll_status::UNINITIALIZED;
		RANDOMIZE(val2_num_of_face, 1);
		int i_result_sum = 0;
		std::ostringstream ostrs_dice_stream(std::ostringstream::ate);
		while (val1_i_num_of_dice > 0)
		{
			int i_step_result = 0;
			RANDOM(i_step_result);
			i_result_sum += i_step_result;
			ostrs_dice_stream << i_step_result;
			if ((--val1_i_num_of_dice) > 0) ostrs_dice_stream << " + ";
		}
		this->pstr_detail_result = new std::string(ostrs_dice_stream.str());
		this->i_sum_result = i_result_sum;
		if (this->status == roll_status::UNINITIALIZED) this->status = roll_status::FINISHED;
	}

	dice_roller::dice_roller(int num_of_dice, int num_of_face, int keep, bool is_keeping_high) {
		this->status = roll_status::UNINITIALIZED;
		if (keep >= num_of_dice) {
			dice_roller dice(num_of_dice, num_of_face);
			this->pstr_detail_result = new std::string(*dice.pstr_detail_result);
			this->i_sum_result = dice.i_sum_result;
		}
		else {
			RANDOMIZE(num_of_face, 1);
			int i_result_sum = 0;
			std::ostringstream ostrs_dice_stream(std::ostringstream::ate);

			std::vector<int> resultList;
			std::vector<int> sortList;
			std::vector<int> pilotList;
			std::vector<int> flagList;

			for (int i_count = 0; i_count < num_of_dice; i_count++) {
				int i_temp_result = 0;
				RANDOM(i_temp_result);
				resultList.push_back(i_temp_result);
				sortList.push_back(i_temp_result);
				pilotList.push_back(i_count);
				flagList.push_back(0);
			}
			quick_sort(sortList.data(), pilotList.data(), 0, sortList.size() - 1);

			if (is_keeping_high) {
				for (int i_iter = 0; i_iter < keep; i_iter++) {
					flagList[pilotList[i_iter]] = 1;
				}
			}
			else {
				for (int i_iter = 1; i_iter <= keep; i_iter++) {
					flagList[pilotList[pilotList.size() - i_iter]] = 1;
				}
			}

			for (int i_iter = 0; i_iter < num_of_dice; i_iter++) {
				if (flagList[i_iter] == 1) {
					i_result_sum += resultList[i_iter];
					ostrs_dice_stream << resultList[i_iter];
				}
				else {
					ostrs_dice_stream << "(" << resultList[i_iter] << ")";
				}
				if (i_iter + 1 < num_of_dice) ostrs_dice_stream << " + ";
			}
			this->pstr_detail_result = new std::string(ostrs_dice_stream.str());
			this->i_sum_result = i_result_sum;
		}
		if (this->status == roll_status::UNINITIALIZED) this->status = roll_status::FINISHED;
	}

	dice_roller::dice_roller(int val1_bp) {
		this->status = roll_status::UNINITIALIZED;
		std::ostringstream ostrs_dice(std::ostringstream::ate);
		ostrs_dice << "d100";
		if(val1_bp > 0)  ostrs_dice << " b" << val1_bp << " = ";
		else if(val1_bp < 0) ostrs_dice << " p" << (-val1_bp) << " = ";

		if(val1_bp == 0){
			int i_result = 0;
			RANDOMIZE(100,1);
			RANDOM(i_result);
			this->i_sum_result = i_result;
			this->status = roll_status::FINISHED;
			this->pstr_detail_result = new std::string(ostrs_dice.str());
		}
		else{
			int i_dice_count = val1_bp > 0 ? (1 + val1_bp):(1 - val1_bp);

			RANDOMIZE(10,0);
			int i_units = 0;
			RANDOM(i_units);
			if(i_units == 0){
				RANDOMIZE(10,1);
			}
			else{
				RANDOMIZE(10,0);
			}
			int i_tens = 0;

			std::vector<int> resultList;
			std::vector<int> sortList;
			std::vector<int> pilotList;
			std::vector<int> flagList;

			for (int i_count = 0; i_count < i_dice_count; i_count++) {
				int i_temp_result = 0;
				RANDOM(i_temp_result);
				resultList.push_back(i_temp_result);
				sortList.push_back(i_temp_result);
				pilotList.push_back(i_count);
				flagList.push_back(0);
			}
			quick_sort(sortList.data(), pilotList.data(), 0, sortList.size() - 1);

			if (val1_bp > 0) {
				flagList[pilotList[0]] = 1;
			}
			else {
				flagList[pilotList[pilotList.size() - 1]] = 1;
			}

			std::ostringstream sstr_tens(std::ostringstream::ate);

			for (int i_iter = 0; i_iter < i_dice_count; i_iter++) {
				if (flagList[i_iter] == 1) {
					i_tens = resultList[i_iter];
					sstr_tens << resultList[i_iter];
				}
				else {
					sstr_tens << "(" << resultList[i_iter] << ")";
				}
				if (i_iter + 1 < i_dice_count) sstr_tens << " + ";
			}

			this->i_sum_result = 10 * i_tens + i_units;

			ostrs_dice << '[' << sstr_tens.str() << "] [" << i_units << "]";
			this->pstr_detail_result = new std::string(ostrs_dice.str());
			this->status = roll_status::FINISHED;
		}
	}

	std::shared_ptr<dice_roller> dice_roller::roll_base(int i_num_of_dice, int i_num_of_face) noexcept{
		if(i_num_of_dice > MAX_DICE_NUM) return nullptr;
		if(i_num_of_face > MAX_DICE_FACE) return nullptr;
		return std::make_shared<dice_roller>(i_num_of_dice,i_num_of_face);
	}

	//only \d*?[d|D]\d+([k|K][l|L]?\d+)? can be used in this function
	std::shared_ptr<dice_roller> dice_roller::roll_rd(std::string & str_dice_command) noexcept{
		do {
			try {
				std::regex regex_rd("(\\d*)?[dD](\\d+)(?:[kK]([lL])?(\\d+))?");
				std::smatch smatch_rd;
				std::regex_match(str_dice_command,smatch_rd,regex_rd);
				if(smatch_rd.begin() == smatch_rd.end()) return nullptr;

				int i_num_of_die = 1;
				int i_num_of_face = 0;
				int i_num_of_keep = 0;
				bool is_keep = false;
				bool is_keeping_high = true;

				if(smatch_rd[1].matched) i_num_of_die = std::stoi(smatch_rd[1].str());
				if(smatch_rd[2].matched) i_num_of_face = std::stoi(smatch_rd[2].str());
				else return nullptr;
				if(smatch_rd[3].matched) is_keeping_high = false;
				if(smatch_rd[4].matched) {
					is_keep = true;
					i_num_of_keep = std::stoi(smatch_rd[4].str());
				}

				if (i_num_of_face > MAX_DICE_FACE || i_num_of_keep > MAX_DICE_FACE || i_num_of_die > MAX_DICE_NUM) {
					return nullptr;
				}

				std::shared_ptr<dice_roller> p_dice;

				if(is_keep){
					p_dice = std::make_shared<dice_roller>(i_num_of_die,i_num_of_face,i_num_of_keep,is_keeping_high);
				}
				else{
					p_dice = std::make_shared<dice_roller>(i_num_of_die,i_num_of_face);
				}

				return p_dice;
			}
			catch (const std::invalid_argument& ia) {
				#ifdef _DEBUG
				logger::log("dice_roller", ia.what());
				#endif
				return nullptr;
			}
		} while (false);
		return nullptr;
	}

	std::shared_ptr<dice_roller> dice_roller::roll_coc(std::string & str_dice_command) noexcept{
		try {
			std::string source(str_dice_command);
			std::regex regex_pb("^([bBpP])(\\d+)");
			std::smatch smatch_coc;
			int i_bp_count = 0;
			while(true){
				std::regex_search(source,smatch_coc,regex_pb);
				if(smatch_coc.begin() == smatch_coc.end()) break;
				int this_bp = std::stoi(smatch_coc[2].str());

				if(smatch_coc[1].str()[0] == 'p' || smatch_coc[1].str()[0] == 'P' ){
					i_bp_count -= this_bp;
				}
				else{
					i_bp_count += this_bp;
				}
				source.assign(smatch_coc.suffix().str());
				if(source.length() < 2) break;
			}
			return std::make_shared<dice_roller>(i_bp_count);
		}
		catch (const std::invalid_argument& ia) {
			#ifdef _DEBUG
				logger::log("dice_roller", ia.what());
			#endif
			return nullptr;
		}
		return nullptr;
	}

	std::shared_ptr<dice_roller> dice_roller::roll_wod(std::string & str_dice_command) noexcept{
		return nullptr;
	}

}