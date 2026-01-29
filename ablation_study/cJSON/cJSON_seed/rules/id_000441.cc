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
//<ID> 441
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *meta = NULL;
    cJSON *parsed = NULL;
    const char *parse_end = NULL;
    char *printed = NULL;
    cJSON *parsed_list = NULL;
    cJSON *parsed_item0 = NULL;
    int arr_size = 0;
    double first_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "single", num);
    cJSON_AddItemReferenceToArray(arr, num);
    cJSON_AddItemToObject(root, "list", arr);
    meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "version", "v1.0");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_ParseWithLengthOpts(printed, (size_t)strlen(printed), &parse_end, 1);
    parsed_list = cJSON_GetObjectItem(parsed, "list");
    arr_size = cJSON_GetArraySize(parsed_list);
    parsed_item0 = cJSON_GetArrayItem(parsed_list, 0);
    first_val = cJSON_GetNumberValue(parsed_item0);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = (char)('A' + (((int)first_val) % 26));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}