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
//<ID> 698
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_a = "{\"message\":\"hello\",\"value\":123}";
    const char *json_b = "{\"name\":\"example\",\"active\":true}";
    const char *end_a = NULL;
    const char *end_b = NULL;
    size_t len_a = strlen(json_a);
    size_t len_b = strlen(json_b);
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON *parsed_a = cJSON_ParseWithLengthOpts(json_a, len_a, &end_a, 1);
    cJSON *parsed_b = cJSON_ParseWithOpts(json_b, &end_b, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON_bool print_ok = 0;
    cJSON *name_item = NULL;
    cJSON_bool name_is_string = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "first", parsed_a);
    cJSON_AddItemToObject(root, "second", parsed_b);
    cJSON_AddBoolToObject(root, "flag", 1);

    // step 3: Operate / Validate
    name_item = cJSON_GetObjectItem(parsed_b, "name");
    name_is_string = cJSON_IsString(name_item);
    print_ok = cJSON_PrintPreallocated(root, buffer, 512, 0);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}