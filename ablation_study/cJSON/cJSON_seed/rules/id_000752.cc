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
//<ID> 752
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
    const char *json_with_extra = "{\"from_parse\":100}EXTRA";
    const char *parse_end = NULL;
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num_item = NULL;
    char *printed = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithOpts(json_with_extra, &parse_end, 0);
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    num_item = cJSON_AddNumberToObject(root, "count", 2.0);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20));

    // step 3: Operate / Validate
    cJSON_AddItemToArray(arr, parsed);
    printed = cJSON_Print(root);
    memcpy(scratch, printed, strlen(printed) + 1);
    arr_size = cJSON_GetArraySize(arr);

    // step 4: Cleanup / Finalize
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}