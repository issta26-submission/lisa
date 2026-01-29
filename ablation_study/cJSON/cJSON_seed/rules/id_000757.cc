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
//<ID> 757
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    char *printed = NULL;
    const char *json = "{\"init_num\":10, \"arr\": [1,2]}";
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *newArr = NULL;
    int arr_size_before = 0;
    int arr_size_after = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithOpts(json, &parse_end, 1);
    newArr = cJSON_CreateArray();
    cJSON_AddItemToArray(newArr, cJSON_CreateNumber(7));
    cJSON_AddItemToArray(newArr, cJSON_CreateNumber(8));
    cJSON_AddItemToObject(root, "newArr", newArr);
    cJSON_AddNumberToObject(root, "added_number", 3.14);

    // step 3: Operate / Validate
    arr_size_before = cJSON_GetArraySize(newArr);
    cJSON_AddItemToArray(newArr, cJSON_CreateNumber(42));
    printed = cJSON_Print(root);
    memcpy(scratch, printed, strlen(printed) + 1);
    arr_size_after = cJSON_GetArraySize(newArr);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}