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
//<ID> 974
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *values = NULL;
    cJSON *added_num = NULL;
    char *unformatted = NULL;
    char *prebuf = NULL;
    char *scratch = NULL;
    const char raw_json[] = "{\"name\":\"device\",\"values\":[1,2,3],\"active\":true}";
    const char *parse_end = NULL;
    const char *ver = NULL;
    int prebuf_len = 256;
    size_t raw_len = sizeof(raw_json) - 1;
    size_t parsed_len = 0;
    cJSON_bool pre_ok = 0;

    // step 2: Setup / Configure
    prebuf = (char *)cJSON_malloc((size_t)prebuf_len);
    memset(prebuf, 0, (size_t)prebuf_len);
    scratch = (char *)cJSON_malloc((size_t)16);
    memset(scratch, 0, (size_t)16);
    root = cJSON_ParseWithLengthOpts(raw_json, raw_len, &parse_end, 1);
    values = cJSON_GetObjectItem(root, "values");
    added_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(values, added_num);

    // step 3: Operate / Validate
    unformatted = cJSON_PrintUnformatted((const cJSON *)root);
    ver = cJSON_Version();
    parsed_len = (size_t)(parse_end - raw_json);
    pre_ok = cJSON_PrintPreallocated(root, prebuf, prebuf_len, 0);
    scratch[0] = unformatted ? unformatted[0] : '\0';
    scratch[1] = ver ? ver[0] : '\0';
    scratch[2] = (char)('0' + (int)(parsed_len % 10));

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(prebuf);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}