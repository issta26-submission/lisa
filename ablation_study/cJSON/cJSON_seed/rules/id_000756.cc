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
//<ID> 756
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
    cJSON *num_in_array = NULL;
    cJSON *num_in_object = NULL;
    cJSON *parsed_clone = NULL;
    int arr_size_before = 0;
    int arr_size_after = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithOpts("{\"root\":{}}", &parse_end, 1);
    arr = cJSON_CreateArray();
    num_in_array = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, num_in_array);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(8.0));
    cJSON_AddItemToObject(root, "numbers", arr);
    num_in_object = cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 3: Operate / Validate
    arr_size_before = cJSON_GetArraySize(arr);
    printed = cJSON_Print(root);
    memcpy(scratch, printed, strlen(printed) + 1);
    parsed_clone = cJSON_ParseWithOpts(scratch, &parse_end, 1);
    arr_size_after = cJSON_GetArraySize(cJSON_GetObjectItem(root, "numbers"));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(parsed_clone);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}