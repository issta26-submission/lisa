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
//<ID> 1419
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *minified_parsed = NULL;
    cJSON *parsed_extra = NULL;
    char *printed = NULL;
    char json1[] = "{\"a\":1, \"b\":[true,null]}";
    char json2[] = "{\"new\":\"value\"}";
    char minify_buf[] = "  { \"z\" : 30 , \"arr\" : [ 1 , 2 , 3 ] }  ";
    size_t minify_len = sizeof(minify_buf) - 1;

    // step 2: Setup / Configure
    root = cJSON_Parse(json1);
    parsed_extra = cJSON_Parse(json2);
    cJSON_Minify(minify_buf);
    minified_parsed = cJSON_ParseWithLength(minify_buf, minify_len);
    cJSON_AddItemToObject(root, "min_obj", minified_parsed);
    cJSON_AddItemToObject(root, "extra", parsed_extra);

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}