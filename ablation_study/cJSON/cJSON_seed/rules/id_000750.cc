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
//<ID> 750
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    char *printed = NULL;
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *parsed = NULL;
    cJSON *parsed_arr = NULL;
    cJSON *dup_item = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    int size_before = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "int_value", 100.0);
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    parsed = cJSON_ParseWithOpts("{\"parsed\":[7,8]}", &parse_end, 1);
    parsed_arr = cJSON_GetObjectItem(parsed, "parsed");
    dup_item = cJSON_Duplicate(cJSON_GetArrayItem(parsed_arr, 0), 1);
    cJSON_AddItemToArray(arr, dup_item);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "imported", parsed);

    // step 3: Operate / Validate
    size_before = cJSON_GetArraySize(arr);
    printed = cJSON_Print(root);
    memcpy(scratch, printed, strlen(printed) + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}