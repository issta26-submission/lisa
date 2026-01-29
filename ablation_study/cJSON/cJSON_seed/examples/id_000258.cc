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
//<ID> 258
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    const char *names_arr[] = { "alice", "bob", "carol" };
    cJSON *names = cJSON_CreateStringArray(names_arr, 3);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "names", names);
    cJSON_AddItemToObjectCS(root, "n1", n1);
    cJSON_AddItemToObjectCS(root, "n2", n2);

    // step 3: Operate & Validate
    cJSON_bool has_n1 = cJSON_HasObjectItem(root, "n1");
    (void)has_n1;
    cJSON *n1_ptr = cJSON_GetObjectItem(root, "n1");
    cJSON *n2_ptr = cJSON_GetObjectItem(root, "n2");
    double v1 = cJSON_GetNumberValue(n1_ptr);
    double v2 = cJSON_GetNumberValue(n2_ptr);
    cJSON *sum = cJSON_CreateNumber(v1 + v2);
    cJSON_AddItemToObjectCS(root, "sum", sum);
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, n2_ptr, replacement);
    (void)replaced;
    cJSON *n2_new_ptr = cJSON_GetObjectItem(root, "n2");
    double v2_new = cJSON_GetNumberValue(n2_new_ptr);
    cJSON *delta = cJSON_CreateNumber(v2_new - v2);
    cJSON_AddItemToObjectCS(root, "delta", delta);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}