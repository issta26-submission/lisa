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
//<ID> 210
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    static const char *names_arr[] = { "Alice", "Bob", "Carol" };
    cJSON *names = cJSON_CreateStringArray(names_arr, 3);
    cJSON_AddItemToObject(root, "names", names);
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 2: Configure
    cJSON_SetNumberHelper(count, 100.5);
    cJSON *names_dup = cJSON_Duplicate(names, 1);
    cJSON_AddItemToObject(root, "names_copy", names_dup);
    cJSON *first_in_dup = cJSON_GetArrayItem(names_dup, 0);
    cJSON *detached = cJSON_DetachItemViaPointer(names_dup, first_in_dup);
    cJSON_AddItemToObject(root, "first_name", detached);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int size_orig = cJSON_GetArraySize(names);
    int size_dup_after = cJSON_GetArraySize(names_dup);
    cJSON *got_first = cJSON_GetObjectItem(root, "first_name");
    char *first_str = cJSON_GetStringValue(got_first);
    double count_val = cJSON_GetNumberValue(count);
    int summary = size_orig + size_dup_after + (first_str ? (int)first_str[0] : 0) + (int)count_val;
    void *tmp_buf = cJSON_malloc(64);
    memset(tmp_buf, 0, 64);
    char *buffered = cJSON_PrintBuffered(root, 128, 0);
    cJSON_free(buffered);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}