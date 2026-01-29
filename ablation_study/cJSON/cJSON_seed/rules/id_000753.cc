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
//<ID> 753
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
    const char *json = "{\"alpha\": 42, \"nested\": {\"beta\": 3}}";
    const char *parse_end = NULL;
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num_item = NULL;
    char *printed = NULL;
    int arr_size_before = 0;
    int arr_size_after = 0;
    double extracted_number = 0.0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithOpts(json, &parse_end, 1);
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "from_parse", parsed);
    cJSON_AddNumberToObject(root, "answer", 100.5);
    num_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, num_item);
    num_item = cJSON_CreateNumber(8.0);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 3: Operate / Validate
    arr_size_before = cJSON_GetArraySize(arr);
    printed = cJSON_PrintUnformatted(root);
    extracted_number = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "answer"));
    num_item = cJSON_CreateNumber(9.0);
    cJSON_AddItemToArray(arr, num_item);
    arr_size_after = cJSON_GetArraySize(arr);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}