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
//<ID> 220
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON *num2 = cJSON_CreateNumber(20.25);
    cJSON *label = cJSON_CreateString("example");
    char *printed = NULL;
    char *extracted_str = NULL;
    cJSON *retrieved_num_item = NULL;
    double extracted_num = 0.0;
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 2: Setup / Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToObject(root, "items", array);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    extracted_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));
    retrieved_num_item = cJSON_GetArrayItem(cJSON_GetObjectItem(root, "items"), 0);
    extracted_num = cJSON_GetNumberValue(retrieved_num_item);
    ((char *)buffer)[0] = extracted_str ? extracted_str[0] : '\0';
    ((char *)buffer)[1] = (char)(int)extracted_num;
    (void)printed;
    (void)retrieved_num_item;
    (void)extracted_str;
    (void)extracted_num;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}