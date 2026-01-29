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
//<ID> 227
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON *str1 = cJSON_CreateString("hello");
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    char *printed = NULL;
    double extracted_number = 0.0;
    char *extracted_string = NULL;
    double computed = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Operate / Validate
    cJSON *retrieved_arr = cJSON_GetObjectItem(root, "items");
    cJSON *retrieved_num = cJSON_GetArrayItem(retrieved_arr, 0);
    cJSON *retrieved_str = cJSON_GetArrayItem(retrieved_arr, 1);
    extracted_number = cJSON_GetNumberValue(retrieved_num);
    extracted_string = cJSON_GetStringValue(retrieved_str);
    computed = extracted_number + 2.5;
    printed = cJSON_PrintUnformatted(root);
    ((char *)buffer)[0] = printed ? printed[0] : '\0';
    (void)extracted_string;
    (void)computed;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}