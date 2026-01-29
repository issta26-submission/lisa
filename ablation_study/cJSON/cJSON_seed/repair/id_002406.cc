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
//<ID> 2406
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_AddStringToObject(root, "name", "test_name");
    const char *simple_json = "{\"parsed\":\"value\",\"count\":7}";
    cJSON *parsed = cJSON_Parse(simple_json);

    // step 2: Configure
    const char *json_with_trailing = "{\"sub\":\"inner\"}TRAIL";
    const char *parse_end = NULL;
    cJSON *parsed2 = cJSON_ParseWithOpts(json_with_trailing, &parse_end, 0);
    char *unformatted = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);

    // step 3: Operate
    cJSON *found_in_parsed = cJSON_GetObjectItem(parsed, "parsed");
    cJSON_bool is_name_string = cJSON_IsString(name_item);
    cJSON_bool is_found_string = cJSON_IsString(found_in_parsed);
    cJSON_bool parsed2_invalid = cJSON_IsInvalid(parsed2);
    cJSON_AddItemToObject(root, "child", parsed2);
    buffer[0] = unformatted ? unformatted[0] : '\0';
    buffer[1] = (char)(int)is_name_string;
    buffer[2] = (char)(int)is_found_string;
    buffer[3] = (char)(int)parsed2_invalid;
    buffer[4] = parse_end ? parse_end[0] : '\0';

    // step 4: Validate and Cleanup
    char *reprinted = cJSON_PrintUnformatted(root);
    buffer[5] = reprinted ? reprinted[0] : '\0';
    cJSON_free(unformatted);
    cJSON_free(reprinted);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}