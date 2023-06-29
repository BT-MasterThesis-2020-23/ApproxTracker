//void dump_flush_controller_l2::remove_pre_existing_files(){
////  char dump_folder[200];
////  sprintf(dump_folder, "rm -rf approximation_trackers/dump_sequence_counter.txt");
////  system(dump_folder);
//
//  char init_dump[200];
//  sprintf(init_dump, "rm -rf approximation_trackers/initial_dump.txt");
//  system(init_dump);
//
//  char flush_tracker[200];
//  sprintf(flush_tracker, "rm -rf approximation_trackers/l2_flush_tracker.csv");
//  system(flush_tracker);
//
//  char old_perf_files[200];
//  sprintf(old_perf_files, "rm -rf approximation_trackers/temp*");
//  system(old_perf_files);
//}
//
//void dump_flush_controller_l2::load_base_infos() 
//{
//  DIR *dr; struct dirent *en;
//	dr = opendir("baseline_results/"); 
//
//  bool file_found = false;
//  if (dr) {
//	  while ((en = readdir(dr)) != NULL) {
//      int cmp_rest = strcmp(en->d_name, app_name);
//  		if (cmp_rest == 0) {
//  			closedir(dr);
//        file_found = true;
//        break;
//		  }
//	  }
//    if (!file_found) {
//      printf("Baseline execution results cannot be found\n");
//      closedir(dr);
//      exit(1);
//    }
//	}
//	else {
//		printf("Directory cannot be opened to check existence of the dump tracer file\n");
//    exit(1);
//	}
//
//  FILE *fptr = NULL;
//  char file_name[256];
//  sprintf(file_name, "baseline_results/%s", app_name);
//  fptr = fopen(file_name, "r");
//  if (!fptr){
//		printf("File %s cannot be opened\n", app_name);
//    exit(1);
//  }
//
//  unsigned tot_kernel = 0;
//  
//  char *kernel_line_detector = (char* ) malloc(sizeof(char) * 256);
//  sprintf(kernel_line_detector, "Destroy streams for kernel %d:", tot_kernel + 1);
//
//  while(!feof(fptr)) {
//    char line[256];
//    fgets(line, 256, fptr);
//
//    if (memcmp(kernel_line_detector, line, strlen(kernel_line_detector)) == 0) 
//    {
//      (tot_kernel)++;
//      free(kernel_line_detector);
//      char *kernel_line_detector = (char* ) malloc(sizeof(char) * 256);
//      sprintf(kernel_line_detector, "Destroy streams for kernel %d:", tot_kernel+ 1);
//    }
//  }
//  free(kernel_line_detector);
//  fclose(fptr);
//  baseline_kernel_amount = tot_kernel;
//}
//
//void dump_flush_controller_l2::load_per_kernel_acc_amounts()
//{
//  bool file_found = 0;
//  DIR *dr; struct dirent *en;
//	dr = opendir("baseline_results/"); 
//
//  if (dr) {
//	  while ((en = readdir(dr)) != NULL) {
//      int cmp_rest = strcmp(en->d_name, app_name);
//  		if (cmp_rest == 0) {
//  			closedir(dr);
//        file_found = 1;
//        break;
//		  }
//	  }
//    if (!file_found) {
//      printf("Baseline execution results cannot be found\n");
//      closedir(dr);
//      exit(1);
//    }
//	}
//	else {
//		printf("Directory cannot be opened to check existence of the dump tracer file\n");
//    exit(1);
//	}
//
//  FILE *fptr = NULL;
//  char file_name[256];
//  sprintf(file_name, "baseline_results/%s", app_name);
//  fptr = fopen(file_name, "r");
//  if (!fptr){
//		printf("File %s cannot be opened\n", app_name);
//    exit(1);
//  }
//  
//  char k_detect[100];
//  sprintf(k_detect, "Destroy streams for kernel %d:", gpu_sim_kernel_id + 1);
//
//  bool portion_found = false;
//  while(!feof(fptr)) 
//  {
//    char line[256];
//    fgets(line, 256, fptr);
//
//    if ((portion_found == false) && (memcmp(k_detect, line, strlen(k_detect)) == 0))
//      portion_found = true;
//
//    else if (portion_found)
//    {
//      if (memcmp("gpu_sim_cycle = ", line, strlen("gpu_sim_cycle = ")) == 0) {
//        char res[256];
//        sprintf(res, "%s", line + strlen("gpu_sim_cycle = "));
//        tot_exe_cycle = strtoull(res, NULL, 10);
//        continue;
//      }
//      if (memcmp("L2_total_cache_accesses = ", line, strlen("L2_total_cache_accesses = ")) == 0) {
//        char res[256];
//        sprintf(res, "%s", line + strlen("L2_total_cache_accesses = "));
//        l2_tot_acc = strtoull(res, NULL, 10);
//        continue;
//      }
//      if (memcmp("L2_total_cache_misses = ", line, strlen("L2_total_cache_misses = ")) == 0) {
//        char res[256];
//        sprintf(res, "%s", line + strlen("L2_total_cache_misses = "));
//        l2_tot_miss = strtoull(res, NULL, 10);
//        continue;
//      }
//      if (memcmp("L2_total_cache_pending_hits = ", line, strlen("L2_total_cache_pending_hits = ")) == 0) {
//        char res[256];
//        sprintf(res, "%s", line + strlen("L2_total_cache_pending_hits = "));
//        l2_tot_pend_hits = strtoull(res, NULL, 10);
//      }
//      if (memcmp("L2_total_cache_reservation_fails = ", line, strlen("L2_total_cache_reservation_fails = ")) == 0) {
//        char res[256];
//        sprintf(res, "%s", line + strlen("L2_total_cache_reservation_fails = "));
//        l2_tot_res_fail = strtoull(res, NULL, 10);
//        break;
//      }
//    }
//  }
//  fclose(fptr);
//}
//
///**  determine flush amount per kernel basis **/
//void dump_flush_controller_l2::determine_flush_amounts() 
//{
//  float err_p = simulator->get_config().m_df_config.error_percentage;
//  unsigned total_flush = ceil((l2_tot_miss / 100) * err_p);
//  unf_flush_amount_per_dump_l2 = ceil(l2_tot_miss / total_flush) + 1;
//  total_flush_carried_on_l2 = ceil(l2_tot_miss / unf_flush_amount_per_dump_l2);
//
//  unf_flush_amount_per_dump_l2 = 400;
//}
//
//void dump_flush_controller_l2::print_baseline_access_amounts(){
//  fflush(stdout);
//  printf("--------------------------------------------------------------\n");
//  printf("L2 access stats (for %d'th kernel) \n", *(this->simulator->kernel_id) + 1);
//  printf("----- \t Total L2 access = %d ----- \n"
//         "----- \t Sector miss + line miss = %d ----- \n" 
//         "----- \t Pending hits = %d ----- \n"
//         "----- \t Reservation failure = %d ----- \n",
//          l2_tot_acc, l2_tot_miss, l2_tot_pend_hits, l2_tot_res_fail);
//
//  printf("Error percentage = %d || total # of flushes for this kernel = %u || dump_frequency (dump per miss) = %u\n", 
//          simulator->get_config().m_df_config.error_percentage, total_flush_carried_on_l2,
//          unf_flush_amount_per_dump_l2);
//  printf("--------------------------------------------------------------\n");
//  fflush(stdout);
//}
//
///**
// * @brief  flush config 1.Without taking care the thread, just flush the access;
// * think about the scenario again.
// */
//
////void dump_flush_controller_l2::determine_flush_amounts() {
//////  unf_flush_amount_per_dump_l1d = ceil(100 / simulator->get_config().m_dump_controller.error_percentage);
////  unf_flush_amount_per_dump_l2 = 500;
//////  printf("Error percentage %u\n", unf_flsh_amount_per_dump_l1d);
////}
//
//
//dump_flush_controller_l2::dump_flush_controller_l2(const gpgpu_sim *sim)
//{
//  //CUDA program name is stored
//  extern char *__progname;
//  sprintf(app_name, "%s.txt", __progname);
//
//  // remove pre existing files
//  remove_pre_existing_files();
//
//  //simulator pointer is re-assigned to a dump/flush controller
//  simulator = sim;
//
//  dump_seq_counter = NULL;
//  l2_flush_tracker = NULL;
//
//  // total kernel number is obtained in the initial kernel
//  // if (gpu_sim_kernel_id == 0) {
//  //   load_base_infos();
//  // }
//
//  if (gpu_sim_kernel_id == 0) {
//    load_per_kernel_acc_amounts();
//    prev_l2_acc = l2_tot_acc;
//    prev_l2_miss_acc = l2_tot_miss;
//    prev_l2_pend_hits = l2_tot_pend_hits;
//    prev_l2_res_fail_acc = l2_tot_res_fail;
//  }
//  else {
//    load_per_kernel_acc_amounts();
//
//    unsigned temp_l2_acc = l2_tot_acc; 
//    unsigned temp_l2_miss_acc = l2_tot_miss; 
//    unsigned temp_l2_pend_hits = l2_tot_pend_hits; 
//    unsigned temp_l2_res_fail_acc = l2_tot_res_fail;   
//
//    l2_tot_acc -= prev_l2_acc;
//    l2_tot_miss -= prev_l2_miss_acc;
//    l2_tot_pend_hits -= prev_l2_pend_hits;
//    l2_tot_res_fail -= prev_l2_res_fail_acc;
//    
//    prev_l2_acc = temp_l2_acc; 
//    prev_l2_miss_acc = temp_l2_miss_acc;
//    prev_l2_pend_hits = temp_l2_pend_hits;
//    prev_l2_res_fail_acc = temp_l2_res_fail_acc;
//  }
//
//  // Determine the flush amount with respect to the error percentage
//  determine_flush_amounts();
//
//  // print experiment details
//  print_baseline_access_amounts();
//
//  if (simulator->get_config().m_df_config.flush_l2) 
//  {
//    total_flush_counter = 0;
//    flush_counter_l2 = 0;
//    char file_name[100];
//    sprintf(file_name, "approximation_trackers/l2_flush_tracker.csv");
//    l2_flush_tracker = fopen(file_name, "w+");
//
//    if (!l2_flush_tracker) {
//      printf("l2_flush_tracker file cannot be opened to record accesses for" 
//              " %d'th kernel\n", gpu_sim_kernel_id);
//      fflush(stdout);
//      exit(1);
//    }
//
//    //tid, pc, cycle, access_mask 
//    fflush(l2_flush_tracker);
//    fprintf(l2_flush_tracker, "cycle,tbid,wid,pc,acc_mask\n");
//    fflush(l2_flush_tracker);
//    after_fl = false;
//  }
//}
//
//void dump_flush_controller_l2::init() 
//{
//  dump_seq_counter = NULL;
//  dump_seq_counter = fopen("approximation_trackers/dump_sequence_counter.txt", "w+");
//  if (dump_seq_counter == NULL) {
//    fflush(stdout);
//    printf("Dump sequence counter text file cannot be opened\n");
//    fflush(stdout);
//    exit(1);
//  }
//
//  fflush(dump_seq_counter);
//  if (gpu_sim_kernel_id == 0)
//    fprintf(dump_seq_counter, "Dumper_0 - C:8000\n");
//  else
//    fprintf(dump_seq_counter, "Dumper_0 - C:5040\n");
//  fflush(dump_seq_counter);
//
//  this->f_access_info.cycle = 0;
//  this->f_access_info.pc = 0;
//  this->f_access_info.tb_id = 0;
//  this->f_access_info.wid = 0;
//  for (unsigned i = 0; i < 32; i++)
//    this->f_access_info.acc_mask[i] = 0;
//}
//
//bool dump_flush_controller_l2::l2_flush_check()
//{
//  if (unf_flush_amount_per_dump_l2 == flush_counter_l2) {
//    flush_counter_l2 = 0;
//    return true;
//  }
//  else {
//    flush_counter_l2++;
//    return false;
//  }
//}
//
//void dump_flush_controller_l2::update_l2_flush_tracker(mem_fetch *mf,
//                                                       unsigned long long cycle) 
//{
//  unsigned tb_id = mf->get_inst().m_ctaid;
//  unsigned pc = mf->get_pc();
//  unsigned wid = mf->get_inst().warp_id();
//
//  fflush(l2_flush_tracker);
//  fprintf(l2_flush_tracker, "%llu,%u,%u,%u,%x\n", cycle, tb_id, wid, pc,
//          mf->get_access_warp_mask());
//  fflush(l2_flush_tracker);
//
//  total_flush_counter++;
//  dump_wait_state(false);
//}
//
//void dump_flush_controller_l2::dump_wait_state(bool after_flush) 
//{
//  if (!after_flush) 
//  {
//    fflush(dump_seq_counter);
//    fprintf(dump_seq_counter, "Dumper_%u\n", total_flush_counter);
//    fflush(dump_seq_counter);
//
//    if (after_fl)
//    {
//      // Create dump @ here.
//      FILE *fptr = NULL;
//      fptr = fopen("approximation_trackers/create_dump_here.txt", "w+");
//
//      if (fptr == NULL)
//        printf("create_dump_here.txt file from simulator side cannot be opened\n");
//
//      fflush(fptr);
//      fprintf(fptr, "Create a dump @ here");
//      fflush(fptr);
//      fclose(fptr);
//
//      while(controller_check((char *)"create_dump_here.txt"));
//
//      after_fl = false;
//    }
//  }
//  else {
//    // Clear access info
//    fflush(stdout);
//    this->f_access_info.cycle = 0;
//    this->f_access_info.pc = 0;
//    this->f_access_info.tb_id = 0;
//    this->f_access_info.wid = 0;
//    free(this->f_access_info.acc_mask_str);
//    // for (unsigned i = 0; i < 32; i++)
//    //   this->f_access_info.acc_mask[i] = 0;
//    after_fl = true;
//    flush_counter_l2 = 0;
//    // printf("After FL is true\n");
//  }
//}
//
//bool dump_flush_controller_l2::controller_check(char* filename){
//
//	DIR *dr;
//	struct dirent *en;
//	dr = opendir("approximation_trackers"); 
//	if (dr) {
//		while ((en = readdir(dr)) != NULL) {
//      int cmp_rest = strcmp(en->d_name, filename);
//			if (cmp_rest == 0) {
//				closedir(dr);
//        return true;
//			}	
//		}
//    closedir(dr);
//    return false;
//	}
//	else {
//		printf("Directory cannot be opened to check existence of %s\n", filename);
//    return false;
//	}
//}
//
//void dump_flush_controller_l2::fetch_flush_access_identifiers()
//{
//  fflush(stdout);
//  FILE *fptr;
//  fptr = fopen("approximation_trackers/temp.csv","r");
//
////  printf("This is the fetch_flush_access_identifiers of l2\n");
//
//  if (!fptr) 
//    printf("temp.csv file cannot be opened on"
//           "the simulator side to fetch access to be flushed\n");
//  
//  char row[256]; 
//  char *token;
//  unsigned row_count = 0;
//  while (feof(fptr) != true) {
//    fgets(row, 256, fptr);
//    if (row_count == (this->total_flush_counter) + 1) {
//      //cycle, tb_id, wid, pc, acc_mask
//      unsigned col_cnt = 0;
//      token = strtok(row, ",");
//      while(token != NULL) {
//        switch(col_cnt) {
//          case 0:
//            this->f_access_info.cycle = atoi(token);                        
//            break;
//          case 1:
//            this->f_access_info.tb_id = atoi(token);
//            break;
//          case 2:
//            this->f_access_info.wid = atoi(token);
//            break;
//          case 3:
//            this->f_access_info.pc = atoi(token);
//            break;
//          case 4:
//            this->f_access_info.acc_mask_str = (char *) malloc(sizeof(char) * 10);
//            strcpy(this->f_access_info.acc_mask_str, token);
//            break;
//        }
//        col_cnt++;
//        token = strtok(NULL, ",");
//      }
//      break;
//    }
//    row_count++;
//  }
//  fclose(fptr);
//
//  unsigned long acc_mask = (unsigned long) strtol(this->f_access_info.acc_mask_str, NULL, 16);
//  for (unsigned i = 0; i < 32; i++) {
//    this->f_access_info.acc_mask[i] = acc_mask % 2;
//    acc_mask = acc_mask / 2;
//  }
//
//  fflush(stdout);
//  printf("tb_id = %u, wid = %u, pc = %u, cycle = %llu, mask = %s",
//          this->f_access_info.tb_id, this->f_access_info.wid, this->f_access_info.pc,
//          this->f_access_info.cycle, this->f_access_info.acc_mask_str);
//  for (int i = 31; i >= 0; i--) {
//    printf("%d", this->f_access_info.acc_mask[i]);
//    if (i % 4 == 0)
//      printf(" ");
//  }
//  printf("\n");
//  fflush(stdout);
//}
//
//dump_flush_controller_l2::~dump_flush_controller_l2(){
//  
//  if (l2_flush_tracker == NULL)
//  {  
//    fflush(stdout);
//    printf("L2 cache flush tracker file deleted for kernel %d\n", gpu_sim_kernel_id);
//    fflush(stdout);
//    fclose(l2_flush_tracker);
//  }
//  if (dump_seq_counter == NULL)
//  {
//    fflush(stdout);
//    printf("L2 cache flush tracker file deleted for kernel %d\n", gpu_sim_kernel_id);
//    fflush(stdout);
//    fclose(dump_seq_counter);
//  }
//}
//
//
//initializer implementation
//flusher checker.
//controller settings.


//-flush_l1d 1 
//-flush_l2 1
//-flush_dram 1


//  config = &(simulator->get_config().m_dump_controller);
//  nof_skip_counter = 0;
//  nof_flushes_l1d = 0;
//  nof_flushes_l2 = 0;
//  nof_flushes_dram = 0;

//  init();

// 2 tür flush yapabiliriz. 

// 1.si l1d'ye olan accessLer üzerinden flush yapmak, 
  // birçok selection configürasyonu düşünülebilir 
  // flushlanabilecek accessler arasından.

// 2.si, randomly thread seçip onlar üzerinden flush yapmak.

// 3.sü application specific thread seçip onlar üzerinden flush yapmak.