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
//<ID> 519
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    const char *json = "{\"config\":{\"flag\":false,\"values\":[1,2,3]}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, strlen(json) + 1, &parse_end, 1);
    cJSON *config = cJSON_GetObjectItem(root, "config");
    cJSON *values = cJSON_GetObjectItem(config, "values");
    int original_size = cJSON_GetArraySize(values);
    cJSON *new_num = cJSON_CreateNumber(99.5);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(values, new_num);
    cJSON_ReplaceItemInObjectCaseSensitive(config, "flag", true_item);
    int new_size = cJSON_GetArraySize(values);
    buffer[0] = (char)('0' + (original_size % 10));
    buffer[1] = ',';
    buffer[2] = (char)('0' + (new_size % 10));
    buffer[3] = '\0';
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    size_t copy_len = printed_len > 10 ? 10 : printed_len;
    memcpy(buffer + 4, printed, copy_len);
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66;
}