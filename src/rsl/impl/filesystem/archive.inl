#pragma once

namespace rsl::filesystem
{
    template <string_like Domain, string_like ... Domains>
    archive::archive(Domain&& domain, Domains&&... domains)
    {
        register_domain(domain);
        (register_domain(domains), ...);
    }
}
