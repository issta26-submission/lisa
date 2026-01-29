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
//<ID> 1452
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(123.5);
    cJSON_AddItemToObject(root, "value", num);

    // step 2: Configure
    cJSON *arr_ref = cJSON_CreateArrayReference(num);
    cJSON_AddItemToObject(root, "ref_arr", arr_ref);

    // step 3: Operate & Validate
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON_bool has_ref = cJSON_HasObjectItem(root, "ref_arr");
    cJSON *orig = cJSON_GetObjectItem(root, "value");
    double orig_val = cJSON_GetNumberValue(orig);
    cJSON *ref_arr_node = cJSON_GetObjectItem(root, "ref_arr");
    cJSON *first_in_ref = cJSON_GetArrayItem(ref_arr_node, 0);
    double ref_val = cJSON_GetNumberValue(first_in_ref);
    (void)printed_ok;
    (void)has_ref;
    (void)orig_val;
    (void)ref_val;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}