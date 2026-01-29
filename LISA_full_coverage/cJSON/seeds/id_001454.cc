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
//<ID> 1454
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(child, "value", num1);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *arr_ref = cJSON_CreateArrayReference(child);
    cJSON_AddItemToObject(root, "refs", arr_ref);
    cJSON *extra_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "extra", extra_num);

    // step 3: Operate & Validate
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON_bool has_refs = cJSON_HasObjectItem(root, "refs");
    cJSON *flag_num = cJSON_CreateNumber((double)has_child + 10.0 * (double)has_refs);
    cJSON_AddItemToObject(root, "flag", flag_num);
    cJSON *retrieved_flag = cJSON_GetObjectItem(root, "flag");
    double flag_val = cJSON_GetNumberValue(retrieved_flag);
    (void)flag_val;
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON *print_status = cJSON_CreateNumber((double)printed_ok);
    cJSON_AddItemToObject(root, "printed_ok", print_status);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}