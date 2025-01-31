#ifndef ZOMBIE_HPP
# define ZOMBIE_HPP

# include <iostream>
# include <string>

class Zombie
{
    public:
		Zombie(std::string name);
		~Zombie(void);
		void	announce(void);
		void	set_name(std::string name);
		void	set_type(std::string type);

	private:
		std::string		_name;
		std::string		_type;

};

#endif