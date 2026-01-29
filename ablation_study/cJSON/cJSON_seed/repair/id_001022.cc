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
//<ID> 1022
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_AddArrayToObject(root, "data");
    cJSON *entry1 = cJSON_CreateObject();
    cJSON_AddNumberToObject(entry1, "id", 1.0);
    cJSON_AddNumberToObject(entry1, "value", 3.14);
    cJSON_AddItemToArray(data, entry1);
    cJSON *entry2 = cJSON_CreateObject();
    cJSON_AddNumberToObject(entry2, "id", 2.0);
    cJSON_AddNumberToObject(entry2, "value", 2.718);
    cJSON_AddItemToArray(data, entry2);

    // step 2: Configure
    double count = (double)cJSON_GetArraySize(data);
    cJSON_AddNumberToObject(root, "count", count);
    cJSON *data_ref = cJSON_GetObjectItem(root, "data");
    cJSON_bool is_arr = cJSON_IsArray(data_ref);

    // step 3: Operate
    void *flag_mem = cJSON_malloc(sizeof(int));
    memset(flag_mem, 0, sizeof(int));
    *((int*)flag_mem) = (int)is_arr;
    cJSON_AddNumberToObject(root, "is_array_flag", (double)(*((int*)flag_mem)));
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_free(flag_mem);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}