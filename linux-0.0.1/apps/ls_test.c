
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	struct dirent entry; //struktura sa kojom radi getdents funkcija
	int current_dir_fd;
	int len;

	//otvaramo trenutni direktorijum za citanje
	current_dir_fd = open(".", O_RDONLY);

	while (1)
	{
		//prvi argument je otvoreni fd za dir koji citamo

		//drugi argument je struktura koja ce sadrzati rezultat

		//treci argument bi trebalo da bude koliko stavki citamo,
		//ali u Linux 0.0.1 se uvek cita jedna stavka i ovaj argument se ignorise

		//ako hocemo da procitamo sve stavke u direktorijumu,
		//moramo da pozovemo getdents vise puta nad istim fd

		//povratna vrednost je duzina imena stavke ili 0 ako smo dosli do kraja
		len = getdents(current_dir_fd, &entry, 1);

		if (len > 0)
		{
			write(1, entry.d_name, len);
			write(1, "\n", 1);
		}
		else
		{
			break;
		}
		
	}

	close(current_dir_fd);

	_exit(0);
}
