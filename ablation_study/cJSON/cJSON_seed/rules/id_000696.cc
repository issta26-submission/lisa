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
//<ID> 696
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    const char *json = "{\"name\":\"tester\",\"active\":true}";
    const char *parse_end1 = NULL;
    const char *parse_end2 = NULL;
    cJSON *parsed_with_length = cJSON_ParseWithLengthOpts(json, strlen(json), &parse_end1, 1);
    cJSON *parsed_with_opts = cJSON_ParseWithOpts(json, &parse_end2, 1);

    // step 2: Setup / Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *dup_parsed = cJSON_Duplicate(parsed_with_length, 1);
    cJSON_AddItemToObject(root, "parsed_dup", dup_parsed);
    cJSON *name_item = cJSON_GetObjectItem(parsed_with_opts, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    cJSON *flag_item = cJSON_AddBoolToObject(root, "name_is_string", name_is_string);

    // step 3: Operate / Validate
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, scratch, 512, 0);

    // step 4: Cleanup
    cJSON_Delete(parsed_with_length);
    cJSON_Delete(parsed_with_opts);
    cJSON_Delete(root);
    cJSON_free(scratch);

    // API sequence test completed successfully
    return 66;
}