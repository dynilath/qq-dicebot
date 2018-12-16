#include "./dice_roller.h"

#include <random>
#include <chrono>

#include "./dice_utils.h"

namespace dicebot::roll{

	bool is_using_pseudo_random = false;
	unsigned long ulong_prand_seed = 0;
	unsigned long ulong_prand_stage = 0;



	#define FUNCTION_PARSE_DICE(_Str_target,_Pos_Start,_Pos_D,_Pos_K,_Pos_L) \
	int i_num_of_die = _Pos_D > 0 ? std::stoi(_Str_target.substr(_Pos_Start, _Pos_D - (_Pos_Start))) : 1;\
	int i_num_of_face = std::stoi(_Str_target.substr(_Pos_D + 1, _Pos_K - (_Pos_D + 1)));\
	int i_num_of_keep = std::stoi(_Str_target.substr(_Pos_L + 1))

	#define CHECK_DICE_LIMITS() \
	if (i_num_of_face > MAX_DICE_FACE || i_num_of_keep > MAX_DICE_FACE || i_num_of_die > MAX_DICE_NUM) { \
		this->status = roll_status::DICE_NOT_AVAILABLE; \
		break; \
	}

	#define CHECK_LIMITS(_Face, _Num) \
	((_Face < MAX_DICE_FACE && _Num < MAX_DICE_NUM) &&\
	(_Face > 0 && _Num > 0))

	#define CREATING_OUTPUT(_Stream,_DiceRoll,_Sign) \
	_Stream << "(" << *(_DiceRoll.pstr_detail_result) << ")";\
	this->i_sum_result = _DiceRoll.i_sum_result * _Sign;\
	this->pstr_detail_result = new std::string(_Stream.str())

	#define _RANDOMIZE(_Face,_Min_Val)\
	std::random_device rd_generator;\
	std::mt19937 mt_generator(ulong_prand_seed);\
	if (is_using_pseudo_random) mt_generator.discard(ulong_prand_stage);\
	std::uniform_int_distribution<> dice(_Min_Val, _Face + _Min_Val - 1)

	#define _RANDOM(_Target)\
	if (is_using_pseudo_random) { _Target = dice(mt_generator); ulong_prand_stage++;}\
	else _Target = dice(rd_generator);

	#define RANDOMIZE(_Max,_Min)\
	std::random_device rd_generator;\
	std::mt19937 mt_generator(ulong_prand_seed);\
	if (dice_roller::is_using_pseudo_random) mt_generator.discard(ulong_prand_stage);\
	std::uniform_int_distribution<> dice(_Min, _Max + _Min - 1)

	#define RANDOM(_Target)\
	if (dice_roller::is_using_pseudo_random) { _Target = dice(mt_generator); ulong_prand_stage++;}\
	else _Target = dice(rd_generator)

	dice_roll roll_base(int const i_num_of_dice, int const i_num_of_face) noexcept{
		_RANDOMIZE(i_num_of_dice,i_num_of_face);
		if(CHECK_LIMITS(i_num_of_dice,i_num_of_face)) {
			int single_result = 0;
			int sum_result = 0;
			std::ostringstream ot(std::ostringstream::ate);
			ot << '[';
			bool first = true;

			int i_dice = i_num_of_dice;
			while(i_dice > 0){
				_RANDOM(single_result);
				sum_result+= single_result;
				ot << single_result;
				if ((--i_dice) > 0) ot << " + ";
			}
			ot<<']';
			return dice_roll(sum_result,ot.str(),roll_status::FINISHED);
		}
		else{
			return dice_roll::ERR_ROLL_EXCEED;
		}
	}

	dice_roll roll_rdk(int const i_num_of_dice, int const i_num_of_face, int const i_keep) noexcept{
		if(i_keep == 0){
			return dice_roll(0,"(0)",roll_status::FINISHED);
		}
		int i_num_of_keep = i_keep >0 ? i_keep :(-i_keep);
		if(i_num_of_keep >= i_num_of_keep) return roll_base(i_num_of_dice,i_num_of_face);


		_RANDOMIZE(i_num_of_face,1);

		if(!CHECK_LIMITS(i_num_of_face,i_num_of_dice)) {
			return dice_roll::ERR_ROLL_EXCEED;
		}

		int i_result = 0;
		std::ostringstream ot(std::ostringstream::ate);
		ot << '[';
		std::vector<int> resultList;
		std::vector<int> sortList;
		std::vector<int> pilotList;
		std::vector<int> flagList;

		for (int i_count = 0; i_count < i_num_of_dice; i_count++) {
			int i_temp_result = 0;
			_RANDOM(i_temp_result);
			resultList.push_back(i_temp_result);
			pilotList.push_back(i_count);
		}
		sortList.assign(resultList.begin(),resultList.end());
		flagList.assign(sortList.size(),0);
		quick_sort(sortList.data(), pilotList.data(), 0, sortList.size() - 1);

		if (i_keep > 0) {
			for (int i_iter = 0; i_iter < i_num_of_keep; i_iter++) {
				flagList[pilotList[i_iter]] = 1;
			}
		}
		else {
			for (int i_iter = 1; i_iter <= i_num_of_keep; i_iter++) {
				flagList[pilotList[pilotList.size() - i_iter]] = 1;
			}
		}

		for (int i_iter = 0; i_iter < i_num_of_dice; i_iter++) {
			if (flagList[i_iter] == 1) {
				i_result += resultList[i_iter];
				ot << resultList[i_iter];
			}
			else {
				ot << "(" << resultList[i_iter] << ")";
			}
			if (i_iter + 1 < i_num_of_dice) ot << " + ";
		}
		ot << ']';
		return dice_roll(i_result,ot.str(),roll_status::FINISHED);
	}

	dice_roll roll_rdk(std::string const & str_dice_command) noexcept{
		try {
			std::string source(str_dice_command);
			std::regex regex_rd("(\\d*)?[dD](\\d+)(?:[kK]([lL])?(\\d+))?");
			std::smatch smatch_rd;
			std::regex_match(str_dice_command,smatch_rd,regex_rd);
			if(smatch_rd.begin() == smatch_rd.end()) return dice_roll::ERR_ROLL_GENERAL;

			int i_num_of_die = 1;
			int i_num_of_face = 0;
			int i_num_of_keep = 0;
			bool is_keep = false;
			bool is_keeping_high = true;

			if(smatch_rd[1].matched) i_num_of_die = std::stoi(smatch_rd[1].str());
			if(smatch_rd[2].matched) i_num_of_face = std::stoi(smatch_rd[2].str());
			else return dice_roll::ERR_ROLL_GENERAL;
			if(smatch_rd[3].matched) is_keeping_high = false;
			if(smatch_rd[4].matched) {
				is_keep = true;
				i_num_of_keep = std::stoi(smatch_rd[4].str());
				if(!is_keeping_high) i_num_of_keep = -i_num_of_keep;
			}

			if(!CHECK_LIMITS(i_num_of_face,i_num_of_die)) return dice_roll::ERR_ROLL_EXCEED;

			if(is_keep){
				return roll_rdk(i_num_of_die,i_num_of_face,i_num_of_keep);
			}return roll_base(i_num_of_die,i_num_of_face);
		}
		catch (const std::invalid_argument& ia) {
			#ifdef _DEBUG
			logger::log("dice_roller", ia.what());
			#endif
			return dice_roll::ERR_ROLL_GENERAL;
		}
	}

	dice_roll roll_coc(int const i_bp) noexcept{
		std::ostringstream ot(std::ostringstream::ate);
		ot << "d100";
		if(i_bp > 0)  ot << " b" << i_bp << " = ";
		else if(i_bp < 0) ot << " p" << (-i_bp) << " = ";

		if(-i_bp == 0){
			int i_result = 0;
			_RANDOMIZE(100,1);
			_RANDOM(i_result);
			return dice_roll(i_result,ot.str(),roll_status::FINISHED);
		}
		else{
			int i_dice_count = i_bp > 0 ? (1 + i_bp):(1 - i_bp);
			int i_units = 0;
			{
				_RANDOMIZE(10,0);
				_RANDOM(i_units);
			}
			std::vector<int> resultList;
			if(i_units == 0){
				_RANDOMIZE(10,1);
				for (int i_count = 0; i_count < i_dice_count; i_count++) {
					int i_temp_result = 0;
					_RANDOM(i_temp_result);
					resultList.push_back(i_temp_result);
				}
				
			}
			else{
				_RANDOMIZE(10,0);
				for (int i_count = 0; i_count < i_dice_count; i_count++) {
					int i_temp_result = 0;
					_RANDOM(i_temp_result);
					resultList.push_back(i_temp_result);
				}
			}
			int i_tens = 0;
			int i_target = 0;
			for(int i = 1;i<i_dice_count;i++){
				if(i_bp > 0 && resultList[i] < resultList[i_target]) {
					i_target = i;
				}
				else if(i_bp < 0 && resultList[1] > resultList[i_target]){
					i_target = i;
				}
			}

			std::ostringstream ot_tens(std::ostringstream::ate);

			for (int i_iter = 0; i_iter < i_dice_count; i_iter++) {
				if (i_iter == i_target) {
					i_tens = resultList[i_iter];
					ot_tens << resultList[i_iter];
				}
				else {
					ot_tens << "(" << resultList[i_iter] << ")";
				}
				if (i_iter + 1 < i_dice_count) ot << " + ";
			}
			ot << '[' << ot_tens.str() << "] [" << i_units << "]";
			return dice_roll(10 * i_tens + i_units, ot.str(), roll_status::FINISHED);
		}
	}

	dice_roll roll_coc(std::string const & str_dice_command) noexcept{
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
			return roll_coc(i_bp_count);
		}
		catch (const std::invalid_argument& ia) {
			#ifdef _DEBUG
				logger::log("dice_roller", ia.what());
			#endif
			return dice_roll::ERR_ROLL_GENERAL;
		}
	}

	void random_initialize(){
		std::random_device rd;
		if (rd.entropy() > 0.0) {
			is_using_pseudo_random = false;
		}
		else is_using_pseudo_random = true;
	}

	dice_roll::dice_roll(int const & i_result, std::string const & str_detail, roll_status const & stat) noexcept{
		this->result = i_result;
		this->detail.assign(str_detail);
		this->status = stat;
	}

	dice_roll::dice_roll(roll_status const & stat) noexcept{
		this->status = stat;
	}

	dice_roll dice_roll::ERR_ROLL_EXCEED(roll_status::GENERAL_ERR);
	dice_roll dice_roll::ERR_ROLL_EXCEED(roll_status::TOO_MANY_DICE);

	explicit dice_roll::operator bool() const noexcept{
		return this->status == roll_status::FINISHED;
	}
}