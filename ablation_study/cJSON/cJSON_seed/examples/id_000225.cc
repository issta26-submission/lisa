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
//<ID> 225
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *label = cJSON_CreateString("example_label");

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.23);
    cJSON *n2 = cJSON_CreateNumber(4.56);
    cJSON *n3 = cJSON_CreateNumber(7.89);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    char *out1 = cJSON_PrintUnformatted(root);
    cJSON_bool has_nums = cJSON_HasObjectItem(root, "numbers");
    cJSON *has_flag = cJSON_CreateBool(has_nums);
    cJSON_AddItemToObject(root, "has_numbers", has_flag);
    void *tmpbuf = cJSON_malloc(128);
    snprintf((char *)tmpbuf, 128, "printed_length=%zu", out1 ? strlen(out1) : 0);
    cJSON *meta = cJSON_CreateString((const char *)tmpbuf);
    cJSON_AddItemToObject(root, "meta", meta);
    char *out2 = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out1);
    cJSON_free(out2);
    cJSON_free(tmpbuf);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}