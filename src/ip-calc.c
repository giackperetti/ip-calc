#include <stdio.h>
#define BYTE 8

union IPAddress {
  long ipv4;
  int ipBytes[4];
};

/* Richiesta del'indirizzo IP da analizzare
 * La funzione usa l'arrow operator(->) per accedere ai membri dell'union
 * IPAddress
 **/
void readIP(union IPAddress *ip, int *e) {
  do {
    printf(
        "\033[1m\e[4mInserisci un indirizzo IP e il suo CIDR:\e[0m\033[0m\n");
    scanf("%d.%d.%d.%d/%d", &ip->ipBytes[0], &ip->ipBytes[1], &ip->ipBytes[2],
          &ip->ipBytes[3], e);
  } while (ip->ipBytes[0] < 0 || ip->ipBytes[0] > 255 || ip->ipBytes[1] < 0 ||
           ip->ipBytes[1] > 255 || ip->ipBytes[2] < 0 || ip->ipBytes[2] > 255 ||
           ip->ipBytes[3] < 0 || ip->ipBytes[3] > 255 || *e < 0 || *e > 32);
}

void IPdecToBin(int n) {
  int binaryNum[BYTE * 4];
  int i;
  for (i = 0; i < BYTE; i++) {
    binaryNum[i] = n % 2;
    n /= 2;
  }
  for (int j = i - 1; j >= 0; j--) {
    printf("%d", binaryNum[j]);
  }
}

void CIDRdecToBin(int n) {
  int binaryNum[BYTE * 4];
  for (int i = 0; i < BYTE * 4; i++) {
    binaryNum[i] = i < n;
  }

  for (int j = 0; j < BYTE * 4; j++) {
    printf("%d", binaryNum[j]);
    if (j < BYTE * 4 - 1 && ((j + 1) % 8) == 0) {
      printf(".");
    }
  }
}

int main() {
  union IPAddress ip;
  int cidr;

  readIP(&ip, &cidr);

  /* IP:
   * Operazione bit a bit che usa il left shift per assegnare
   * correttamente e mettere in ordine la posizione dei bit dell'ip
   **/
  long IP = (ip.ipBytes[0] << 24) | (ip.ipBytes[1] << 16) |
            (ip.ipBytes[2] << 8) | ip.ipBytes[3];

  /* subnetMask:
   * 0xFFFFFFFF rappresenta un intero a 32bit con tutti i bit impostati a '1'
   * Si usa il left shift per determinare il numero di bit degli host
   * (32(massimo di bit) - cidr(valore dei bit della parte di rete))
   **/
  long subnetMask = (0xFFFFFFFF << (32 - cidr));

  /* networkIP:
   * Calcola l'indirizzo di rete attraverso un operazione bit
   * a bit AND su IP(ricevuto in input) e Subnet Mask
   **/
  long networkIP = IP & subnetMask;

  /* hostBits:
   * Calcola il valore binario degli host impostando i bit dell'host
   * a '1' e il resto a '0' utilizzando un right shift su 0xFFFFFFFF.
   **/
  long hostBits = 0xFFFFFFFF >> cidr;

  /* firstHostIP:
   * Calcola l'indirizzo del primo host aggiungendo 1 all'indirizzo di rete.
   **/
  long firstHostIP = networkIP + 1;

  /* lastHostIP:
   * Calcola l'indirizzo dell'ultimo host effettuando un'operazione di OR
   * tra l'indirizzo di rete e i bit degli host, quindi sottraendo 1.
   **/
  long lastHostIP = (networkIP | hostBits) - 1;

  /* broadcastIP:
   * Calcola l'indirizzo di broadcast effettuando un'operazione di OR
   * tra l'indirizzo di rete e i bit degli host.
   **/
  long broadcastIP = networkIP | hostBits;

  // stampa di IP e CIDR(subnet mask) sotto forma di bit
  printf("\n\033[1m\e[4mIP sotto forma di bit:\e[0m\033[0m\n");
  IPdecToBin(ip.ipBytes[0]);
  printf(".");
  IPdecToBin(ip.ipBytes[1]);
  printf(".");
  IPdecToBin(ip.ipBytes[2]);
  printf(".");
  IPdecToBin(ip.ipBytes[3]);
  printf("\n\033[1m\e[4mCIDR sotto forma di bit:\e[0m\033[0m\n");
  CIDRdecToBin(cidr);

  /* Stampa dei valori finali:
   * Si divide l'indirizzo IP in 4 sezioni e si stampano i valori
   * Per ogni sezione dei vari indirizzi IP vengono eseguite 3 operazioni:
   * - Si applica un right shift che assicura che i 4 byte dell'IP siano
   *   nell'ordine corretto, dal piu' significativo al meno
   *   significativo
   *
   * - Si applica un operazione di mascheramento che fa si che vengano
   *   conservati i bit meno significativi ancora da trattare
   *   (0xFF rappresenta un byte con tutti i bit impostati a '1')
   *
   * - Si utilizza un cast per assicurare che il valore venga ritornato
   *sotto forma di intero
   **/

  printf("\n\n\033[1m\e[4mIP di Rete:\e[0m\033[0m %d.%d.%d.%d\n",
         (int)((networkIP >> 24) & 0xFF), (int)((networkIP >> 16) & 0xFF),
         (int)((networkIP >> 8) & 0xFF), (int)(networkIP & 0xFF));
  printf("\033[1m\e[4mIP Primo Host:\e[0m\033[0m %d.%d.%d.%d\n",
         (int)((firstHostIP >> 24) & 0xFF), (int)((firstHostIP >> 16) & 0xFF),
         (int)((firstHostIP >> 8) & 0xFF), (int)(firstHostIP & 0xFF));
  printf("\033[1m\e[4mIP Ultimo Host:\e[0m\033[0m %d.%d.%d.%d\n",
         (int)((lastHostIP >> 24) & 0xFF), (int)((lastHostIP >> 16) & 0xFF),
         (int)((lastHostIP >> 8) & 0xFF), (int)(lastHostIP & 0xFF));
  printf("\033[1m\e[4mIP Broadcast:\e[0m\033[0m %d.%d.%d.%d\n",
         (int)((broadcastIP >> 24) & 0xFF), (int)((broadcastIP >> 16) & 0xFF),
         (int)((broadcastIP >> 8) & 0xFF), (int)(broadcastIP & 0xFF));

  return 0;
}
