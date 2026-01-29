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
//<ID> 1261
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToArray(numbers, n3);

    // step 3: Operate
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(numbers, 1, replacement);
    cJSON *got_numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *second_item = cJSON_GetArrayItem(got_numbers, 1);
    cJSON_bool is_number = cJSON_IsNumber(second_item);
    (void)replaced;
    (void)is_number;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed) + 1;
    char *buf = (char *)cJSON_malloc(printed_len);
    memset(buf, 0, printed_len);
    memcpy(buf, printed, printed_len);
    cJSON_Minify(buf);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}