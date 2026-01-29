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
//<ID> 515
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"arr\":[1,2,3],\"flag\":false,\"value\":0}";
    const char *parse_end = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_ParseWithLengthOpts(json, (size_t)(sizeof(json) - 1), &parse_end, 1);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *new_num = cJSON_CreateNumber(99.5);
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Setup / Configure
    cJSON_ReplaceItemInObjectCaseSensitive(root, "value", new_num);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", true_item);

    // step 3: Operate / Validate
    int arr_size = cJSON_GetArraySize(arr);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = '\0';
    double updated_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    char *printed = cJSON_PrintUnformatted(root);
    buffer[2] = printed ? printed[0] : '\0';
    (void)updated_value; // used to show data flow from replacement

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}