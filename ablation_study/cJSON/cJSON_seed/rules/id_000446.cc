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
//<ID> 446
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
    cJSON *num = cJSON_CreateNumber(13.37);
    cJSON *meta = NULL;
    cJSON *parsed = NULL;
    const char *parse_end = NULL;
    char *printed = NULL;
    cJSON *retrieved_arr = NULL;
    int arr_size = 0;
    const char *author = NULL;
    size_t printed_len = 0;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "name", "example");
    meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "author", "tester");
    cJSON_AddItemReferenceToArray(arr, num);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = strlen(printed) + 1; /* include null terminator for ParseWithLengthOpts */
    parsed = cJSON_ParseWithLengthOpts(printed, printed_len, &parse_end, 1);
    retrieved_arr = cJSON_GetObjectItem(parsed, "values");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    author = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(parsed, "meta"), "author"));
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = author[0];
    buffer[2] = '_';
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}