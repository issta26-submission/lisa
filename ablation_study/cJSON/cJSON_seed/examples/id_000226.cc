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
//<ID> 226
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

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *greet = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greet);
    cJSON_bool has_numbers = cJSON_HasObjectItem(root, "numbers");
    cJSON *has_flag = cJSON_CreateNumber((double)has_numbers);
    cJSON_AddItemToObject(root, "has_numbers", has_flag);

    // step 3: Operate & Validate
    int buf_len = 512;
    char *buf = (char*)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON *printed_flag = cJSON_CreateNumber((double)printed);
    cJSON_AddItemToObject(root, "printed", printed_flag);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}