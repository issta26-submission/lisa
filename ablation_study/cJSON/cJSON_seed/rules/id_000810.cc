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
//<ID> 810
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *dup = NULL;
    cJSON *raw = NULL;
    cJSON *flag = NULL;
    cJSON *fetched_raw = NULL;
    cJSON *cmp_node = NULL;
    char buffer[256];
    const int buffer_len = sizeof(buffer);
    const char *raw_text = "{\"meta\":true}";
    cJSON_bool print_ok = 0;
    cJSON_bool cmp_res = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    raw = cJSON_CreateRaw(raw_text);
    cJSON_AddItemToObject(root, "meta_raw", raw);
    flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate / Validate
    dup = cJSON_Duplicate(root, 1);
    memset(buffer, 0, buffer_len);
    print_ok = cJSON_PrintPreallocated(dup, buffer, buffer_len, 1);
    fetched_raw = cJSON_GetObjectItemCaseSensitive(dup, "meta_raw");
    cmp_res = cJSON_Compare(raw, fetched_raw, 1);
    cmp_node = cJSON_CreateBool(cmp_res);
    cJSON_AddItemToObject(root, "cmp_equal", cmp_node);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)print_ok;
    (void)buffer;
    return 66;
}