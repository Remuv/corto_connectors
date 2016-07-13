

set(command "${make};install")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/home/hendren/dev/remuv/src/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/bsoncxx/third_party/EP_mnmlstc_core-prefix/src/EP_mnmlstc_core-stamp/EP_mnmlstc_core-install-out.log"
  ERROR_FILE "/home/hendren/dev/remuv/src/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/bsoncxx/third_party/EP_mnmlstc_core-prefix/src/EP_mnmlstc_core-stamp/EP_mnmlstc_core-install-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /home/hendren/dev/remuv/src/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/bsoncxx/third_party/EP_mnmlstc_core-prefix/src/EP_mnmlstc_core-stamp/EP_mnmlstc_core-install-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "EP_mnmlstc_core install command succeeded.  See also /home/hendren/dev/remuv/src/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/bsoncxx/third_party/EP_mnmlstc_core-prefix/src/EP_mnmlstc_core-stamp/EP_mnmlstc_core-install-*.log")
  message(STATUS "${msg}")
endif()
