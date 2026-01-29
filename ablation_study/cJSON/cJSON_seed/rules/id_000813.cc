#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 813
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *raw = NULL;
    cJSON *flag_false = NULL;
    cJSON *dup_root = NULL;
    cJSON *got_raw = NULL;
    cJSON *cmpnum = NULL;
    char buffer[256];
    cJSON_bool print_ok = 0;
    cJSON_bool compare_result = 0;
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    raw = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw);
    flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag_false);
    dup_root = cJSON_Duplicate(root, 1);

    // step 3: Operate / Validate
    got_raw = cJSON_GetObjectItemCaseSensitive(root, "meta_raw");
    print_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    compare_result = cJSON_Compare(root, dup_root, 1);
    cmpnum = cJSON_CreateNumber((double)compare_result);
    cJSON_AddItemToObject(root, "equal", cmpnum);
    (void)got_raw;
    (void)print_ok;

    // step 4: Cleanup
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}