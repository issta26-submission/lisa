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
//<ID> 976
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json_literal[] = "{\"name\":\"tester\",\"value\":123.5,\"arr\":[true,false,null]}";
    size_t json_len = sizeof(json_literal) - 1;
    const char *parse_end = NULL;
    const char *version = NULL;
    const char *name_str = NULL;
    double valnum = 0.0;
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *value_item = NULL;
    cJSON *arr_item = NULL;
    char *unformatted = NULL;
    char prebuffer[256];
    cJSON_bool pre_ok = 0;

    // step 2: Setup / Configure
    version = cJSON_Version();
    root = cJSON_ParseWithLengthOpts(json_literal, json_len, &parse_end, 1);
    name_item = cJSON_GetObjectItem(root, "name");
    value_item = cJSON_GetObjectItem(root, "value");
    arr_item = cJSON_GetObjectItem(root, "arr");

    // step 3: Operate / Validate
    unformatted = cJSON_PrintUnformatted((const cJSON *)root);
    memset(prebuffer, 0, sizeof(prebuffer));
    pre_ok = cJSON_PrintPreallocated(root, prebuffer, (int)sizeof(prebuffer), 0);
    name_str = cJSON_GetStringValue(name_item);
    valnum = cJSON_GetNumberValue(value_item);
    prebuffer[0] = name_str ? name_str[0] : '\0';
    prebuffer[1] = (char)('0' + (int)pre_ok);
    prebuffer[2] = version ? version[0] : '\0';
    prebuffer[3] = parse_end ? *parse_end : '\0';

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}