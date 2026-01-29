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
//<ID> 754
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
    const char *json_text = "{\"name\":\"fuzz\",\"value\":42}";
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *parsed = NULL;
    int arr_size_before = 0;
    int arr_size_after = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON_AddItemToArray(arr, parsed);
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Operate / Validate
    arr_size_before = cJSON_GetArraySize(arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(7.0));
    arr_size_after = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)arr_size_after);
    printed = cJSON_Print(root);
    memcpy(scratch, printed, strlen(printed) + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}