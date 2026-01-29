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
//<ID> 127
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *raw = cJSON_CreateRaw("{\"raw\":true}");
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *dup = NULL;
    char *printed = NULL;
    void *tmp_mem = NULL;
    cJSON_bool cmp_result = 0;
    cJSON_bool is_raw_flag = 0;
    double read_value = 0.0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToObject(root, "child", child);
    n1 = cJSON_AddNumberToObject(child, "value", 42.0);
    cJSON_AddItemToObject(root, "raw", raw);
    dup = cJSON_Duplicate(root, 1);
    tmp_mem = cJSON_malloc((size_t)32);

    // step 3: Operate / Validate
    read_value = cJSON_GetNumberValue(n1);
    cmp_result = cJSON_Compare(root, dup, 1);
    is_raw_flag = cJSON_IsRaw(cJSON_GetObjectItem(root, "raw"));
    n2 = cJSON_AddNumberToObject(root, "cmp_as_num", (double)cmp_result);
    cJSON_AddNumberToObject(root, "is_raw_num", (double)is_raw_flag);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_mem);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}