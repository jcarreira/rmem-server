#include "common.h"
#include "build.h"
#include "packingDNAseq.h"

int build(gen_state_t *state, char *input_UFX_name)
{
    rvm_txid_t txid;
    build_state_t *bstate;

   /* Initialize lookup table that will be used for the DNA packing routines */
   init_LookupTable();

   /* Extract the number of k-mers in the input file */
   int64_t nKmers = getNumKmersInUFX(input_UFX_name);

   assert(state->phase == BUILD);

   if(state->pstate == NULL) {
       /* We need to initialize build */
       TX_START(txid);

       state->phase = BUILD;
       state->tbl = create_hash_table(nKmers, state->memheap);
       state->pstate = rvm_alloc(cfg, sizeof(build_state_t));
       CHECK_ERROR(state->pstate == NULL, ("Failed to allocate build state\n"));
       bstate = (build_state_t*)state->pstate;
       bstate->kmerx = 0;

       TX_COMMIT(txid);
   }

   /* Read the kmers from the input file and store them in the working_buffer */
   /* TODO We should probably MMAP this so that restarting is faster...*/
   int64_t total_chars_to_read = nKmers * LINE_SIZE;
   unsigned char *working_buffer = (unsigned char*) malloc(total_chars_to_read * sizeof(unsigned char));
   FILE *inputFile = fopen(input_UFX_name, "r");
   int64_t cur_chars_read = fread(working_buffer, sizeof(unsigned char),total_chars_to_read , inputFile);
   fclose(inputFile);

   /* Process the working_buffer and store the k-mers in the hash table */
   /* Expected format: KMER LR ,i.e. first k characters that represent the kmer, then a tab and then two chatacers, one for the left (backward) extension and one for the right (forward) extension */

   int64_t ptr = bstate->kmerx * LINE_SIZE;
   while (ptr < cur_chars_read) {
       TX_START(txid);
      /* working_buffer[ptr] is the start of the current k-mer                */
      /* so current left extension is at working_buffer[ptr+KMER_LENGTH+1]    */
      /* and current right extension is at working_buffer[ptr+KMER_LENGTH+2]  */

      char left_ext = (char) working_buffer[ptr+KMER_LENGTH+1];
      char right_ext = (char) working_buffer[ptr+KMER_LENGTH+2];

      /* Add k-mer to hash table */
      add_kmer(state->tbl, state->memheap,
              &working_buffer[ptr], left_ext, right_ext);

      /* Create also a list with the "start" kmers: nodes with F as left (backward) extension */
      if (left_ext == 'F') {
         addKmerToStartList(state->memheap, &(state->start_list));
      }

      /* Move to the next k-mer in the input working_buffer */
      ptr += LINE_SIZE;
      bstate->kmerx++;

      TX_COMMIT(txid);
   }

   return 1;
}

void build_state_free(void *build_state)
{
    rvm_free(cfg, build_state);
}
