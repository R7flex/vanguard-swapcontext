void __fastcall SwapContextHook(__int64 current_thread)
{
  unsigned __int64 cr3; // rax
  char write_cr3; // bl
  __int64 v4; // rdx

  ((void (*)(void))(qword_140078630 ^ qword_140078638[(unsigned __int8)byte_140078629]))();// importer syscall
  cr3 = __readcr3();
  if ( cr3 != game_cr3
    || ((__int64 (__fastcall *)(__int64))(qword_14007AED0 ^ qword_14007AED8[HIBYTE(word_14007AEC8)]))(current_thread) != game_process )// PsGetThreadProcess call and sure to game
  {
    return;
  }
  _disable();                                   // disabling interrupts
  sub_14000E738(original_pml4);                 // another syscall
  ((void (__fastcall *)(__int64, __int64, __int64))memmove)(clone_pml4, original_pml4, 0x1000i64);
  write_cr3 = 0;
  *(_QWORD *)(clone_pml4 + 8i64 * (unsigned int)free_pml4_index) = shadow_pml4;
  KeAcquireSpinLockAtDpcLevel(&SpinLock);
  if ( threads_number == 512 )
    goto array;
  v4 = 0i64;
  if ( threads_number )
  {
    while ( current_thread != *(_QWORD *)(thread_array + 8 * v4) )
    {
      v4 = (unsigned int)(v4 + 1);
      if ( (unsigned int)v4 >= threads_number )
        goto task_last;
    }
array:
    write_cr3 = 1;
  }
task_last:
  KeReleaseSpinLockFromDpcLevel(&SpinLock);
  if ( write_cr3 )
    __writecr3(clone_cr3);
  if ( possible_flush_tlb )
    flush_tlb();
  _enable();                                    // enabling interrupts
}
