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
//<ID> 532
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    const char *json = "{\"person\":{\"name\":\"Alice\",\"age\":30},\"active\":true}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, strlen(json) + 1, &parse_end, 1);

    // step 2: Setup / Configure
    cJSON *person = cJSON_GetObjectItemCaseSensitive(root, "person");
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(person, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *nickname = cJSON_CreateString("Ally");
    cJSON_AddItemToObject(person, "nickname", nickname);

    // step 3: Operate / Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t name_len = strlen(name_str);
    size_t printed_len = strlen(printed);
    size_t copy_name = name_len > 10 ? 10 : name_len;
    memcpy(buffer, name_str, copy_name);
    size_t offset = copy_name;
    buffer[offset] = ',';
    offset += 1;
    size_t copy_print = printed_len > (256 - offset - 1) ? (256 - offset - 1) : printed_len;
    memcpy(buffer + offset, printed, copy_print);
    buffer[offset + copy_print] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}