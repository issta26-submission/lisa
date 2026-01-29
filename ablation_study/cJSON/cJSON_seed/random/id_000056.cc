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
//<ID> 56
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *source = "  { \"alpha\" : true, \"beta\" : false }  ";
    size_t src_len = strlen(source);
    char *buffer = (char *)cJSON_malloc(src_len + 1);
    memset(buffer, 0, src_len + 1);
    memcpy(buffer, source, src_len + 1);
    size_t buflen = src_len;

    // step 2: Minify the JSON in-place
    cJSON_Minify(buffer);

    // step 3: Parse with length
    cJSON *root = cJSON_ParseWithLength(buffer, buflen);

    // step 4: Configure and operate - create subobject and add booleans
    cJSON *sub = cJSON_CreateObject();
    cJSON_AddBoolToObject(sub, "inner_flag", 0);
    cJSON_AddItemToObjectCS(root, "sub", sub);
    cJSON_AddBoolToObject(root, "is_object", cJSON_IsObject(root));

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}