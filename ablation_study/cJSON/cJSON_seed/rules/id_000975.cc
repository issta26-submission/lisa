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
//<ID> 975
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_text = "{\"name\":\"device\",\"value\":123,\"enabled\":true}";
    size_t json_len = strlen(json_text);
    const char *parse_end = NULL;
    const char *version = NULL;
    cJSON *root = NULL;
    cJSON *item_name = NULL;
    cJSON *item_value = NULL;
    char *unformatted = NULL;
    int prebuf_len = 128;
    char *prebuf = NULL;
    cJSON_bool prealloc_ok = 0;
    double extracted_value = 0.0;

    // step 2: Setup / Configure
    version = cJSON_Version();
    root = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);

    // step 3: Operate / Validate
    unformatted = cJSON_PrintUnformatted((const cJSON *)root);
    prebuf = (char *)cJSON_malloc((size_t)prebuf_len);
    memset(prebuf, 0, (size_t)prebuf_len);
    prealloc_ok = cJSON_PrintPreallocated(root, prebuf, prebuf_len, 0);
    item_value = cJSON_GetObjectItem(root, "value");
    extracted_value = cJSON_GetNumberValue(item_value);
    item_name = cJSON_GetObjectItem(root, "name");
    prebuf[0] = (char)('0' + ((int)extracted_value % 10));
    prebuf[1] = item_name ? (item_name->valuestring ? item_name->valuestring[0] : '\0') : '\0';
    prebuf[2] = version ? version[0] : '\0';
    (void)prealloc_ok;
    (void)parse_end;
    (void)unformatted;

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}