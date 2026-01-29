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
//<ID> 550
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"device\":{\"name\":\"Sensor\",\"value\":42}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, strlen(json) + 1, &parse_end, 1);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);

    // step 2: Setup / Configure
    cJSON *device = cJSON_GetObjectItem(root, "device");
    cJSON *name_item = cJSON_GetObjectItem(device, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *created_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", created_true);
    cJSON *added_num = cJSON_AddNumberToObject(root, "added_number", 3.14159);
    cJSON *str_ref = cJSON_CreateStringReference("ref_string");
    cJSON_AddItemToObject(root, "ref", str_ref);

    // step 3: Operate / Validate
    char *ref_str = cJSON_GetStringValue(str_ref);
    char first_name_char = name_str[0];
    char first_ref_char = ref_str[0];
    buffer[0] = first_name_char;
    buffer[1] = ',';
    buffer[2] = first_ref_char;
    buffer[3] = '\0';
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    size_t copy_len = printed_len < 240 ? printed_len : 240;
    memcpy(buffer + 4, printed, copy_len);
    buffer[4 + copy_len] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}