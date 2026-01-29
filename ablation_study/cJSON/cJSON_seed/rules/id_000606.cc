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
//<ID> 606
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    const char *json_text = "{\"name\":\"Alice\",\"age\":30}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *raw = cJSON_CreateRaw("\"unescaped:\\\"raw\\\"\"");
    size_t printed_len = 0;
    size_t to_copy = 0;
    cJSON *parsed_again = NULL;
    cJSON *name_item = NULL;
    const char *name_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "nickname", "Ally");
    cJSON_AddItemToObject(root, "extra_raw", raw);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 255 ? printed_len : 255;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    parsed_again = cJSON_Parse(scratch);
    name_item = cJSON_GetObjectItem(parsed_again, "name");
    name_str = name_item ? cJSON_GetStringValue(name_item) : NULL;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(parsed_again);

    // API sequence test completed successfully
    return 66;
}