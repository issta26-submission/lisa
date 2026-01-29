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
//<ID> 694
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_person = "{\"name\":\"Alice\",\"age\":30}";
    const char *json_job = "{\"title\":\"Engineer\",\"level\":2}";
    size_t len_person = strlen(json_person);
    size_t len_job = strlen(json_job);
    const char *endptr_person = NULL;
    const char *endptr_job = NULL;
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    char *outbuf = (char *)cJSON_malloc(128);
    memset(outbuf, 0, 128);
    cJSON *parsed_person = NULL;
    cJSON *parsed_job = NULL;
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *title_item = NULL;
    cJSON_bool name_is_str = 0;
    cJSON_bool title_is_str = 0;
    cJSON_bool print_ok = 0;

    // step 2: Setup / Configure
    parsed_person = cJSON_ParseWithLengthOpts(json_person, len_person, &endptr_person, 1);
    parsed_job = cJSON_ParseWithOpts(json_job, &endptr_job, 1);
    root = cJSON_CreateObject();
    cJSON_AddItemReferenceToObject(root, "person", parsed_person);
    cJSON_AddItemReferenceToObject(root, "job", parsed_job);

    // step 3: Operate / Validate
    name_item = cJSON_GetObjectItem(parsed_person, "name");
    title_item = cJSON_GetObjectItem(parsed_job, "title");
    name_is_str = cJSON_IsString(name_item);
    title_is_str = cJSON_IsString(title_item);
    cJSON_AddBoolToObject(root, "name_is_string", name_is_str);
    cJSON_AddBoolToObject(root, "title_is_string", title_is_str);
    print_ok = cJSON_PrintPreallocated(root, outbuf, 128, 0);
    memcpy(scratch, outbuf ? outbuf : "", (outbuf && strlen(outbuf) < 256) ? strlen(outbuf) : 0);

    // step 4: Cleanup
    cJSON_free(outbuf);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}