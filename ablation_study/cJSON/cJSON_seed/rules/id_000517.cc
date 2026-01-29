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
//<ID> 517
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
    const char *sample = "{\"config\":{\"enabled\":false,\"value\":10},\"list\":[1,2,3]}";
    strcpy(buffer, sample);
    size_t len = strlen(buffer);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(buffer, len, &parse_end, 1);
    cJSON *config = cJSON_GetObjectItem(root, "config");
    cJSON *list = cJSON_GetObjectItem(root, "list");

    // step 2: Setup / Configure
    cJSON *replacement_true = cJSON_CreateTrue();
    cJSON *replacement_number = cJSON_CreateNumber(99.5);
    cJSON_ReplaceItemInObjectCaseSensitive(config, "enabled", replacement_true);
    cJSON_ReplaceItemInObjectCaseSensitive(config, "value", replacement_number);

    // step 3: Operate / Validate
    int list_size = cJSON_GetArraySize(list);
    buffer[0] = (char)('0' + (char)list_size);
    buffer[1] = (parse_end && parse_end[0]) ? parse_end[0] : '\0';
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}