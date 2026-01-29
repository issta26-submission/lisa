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
//<ID> 424
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *tmp_buf = (char *)cJSON_malloc(64);
    memset(tmp_buf, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(25.0);
    cJSON *greeting = cJSON_CreateString("hi");
    cJSON *parsed = NULL;
    cJSON *retrieved_arr = NULL;
    cJSON *first_item = NULL;
    int arr_size = 0;
    double first_value = 0.0;
    const char *gstr = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retrieved_arr = cJSON_GetObjectItem(parsed, "numbers");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    first_item = cJSON_GetArrayItem(retrieved_arr, 0);
    first_value = cJSON_GetNumberValue(first_item);
    gstr = cJSON_GetStringValue(cJSON_GetObjectItem(parsed, "greeting"));
    tmp_buf[0] = (char)('0' + (arr_size % 10));
    tmp_buf[1] = gstr[0];
    tmp_buf[2] = (char)('0' + ((int)first_value % 10));
    tmp_buf[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}